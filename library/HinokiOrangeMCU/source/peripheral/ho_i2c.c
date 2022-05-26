/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/peripheral/ho_i2c.h>
#include <ho/ho_time.h>

/* DEBUG -------------------------------------------------------------------- */
ho_cstring HO_COMP_I2C = "I2C";

/* API ---------------------------------------------------------------------- */
void ho_i2c_scan(struct ho_i2c_bus *self,
                 ho_u32 timeout,
                 void (*found)(ho_u16)) {
  for (ho_u16 addr = 0; addr < 0x7F; addr++) {
    if (ho_i2c_ack(self, addr, timeout) && found) { found(addr); }
  }
}

/* CLI ---------------------------------------------------------------------- */
static struct ho_i2c_bus *cmd_i2c_bus = NULL;
static struct ho_cli_parser *cmd_i2c_parser = NULL;
static ho_u16 cmd_i2c_addr = 0;
/* -------------------------------------------------------------------------- */
HO_CLI_CMD_CALLBACK(ho_cli_cmd_i2c_callback) {
  cmd_i2c_bus = cmd->payload;
  cmd_i2c_parser = parser;
  return true;
}
/* -------------------------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_cli_cmd, ho_cli_cmds_i2c);
struct ho_cli_cmds ho_cli_cmds_i2c = HO_LIST_STRUCT_MAKE(ho_cli_cmds_i2c);

/* ---------------------------------- Scan ---------------------------------- */
static void cmd_scan_found_callback(ho_u16 addr) {
  ho_cli_ok(cmd_i2c_parser, "0x%04X", addr);
}
HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmds_i2c, ho_cli_cmd_i2c_scan, "scan") {
  ho_i2c_scan(cmd_i2c_bus, HO_MS(10), cmd_scan_found_callback);
  return true;
}

/* DEV ---------------------------------------------------------------------- */
HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmds_i2c,
                            ho_cli_cmd_i2c_dev,
                            "-d",
                            .flag = HO_CMD_PARAM | HO_CMD_MATCH_PREFIX) {
  if (sscanf(raw, "-d%hX", &cmd_i2c_addr) == 1) {
    ho_cli_ok(parser, "Device=0x%04X", cmd_i2c_addr);
    return true;
  } else {
    ho_cli_error(parser, "Can't parsed device address");
    return false;
  }
}

/* TRANSFER ----------------------------------------------------------------- */
static ho_u8 wdata[16] = { 0 };
static ho_u8 rdata[16] = { 0 };
static ho_u32 wlen = 0;
static ho_u32 rlen = 0;

HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmds_i2c,
                            ho_cli_cmd_i2c_write,
                            "-w",
                            .flag = HO_CMD_MATCH_PREFIX | HO_CMD_BATCH) {
  ho_u16 tmp;

  raw += 2; /** skip '-w' */
  wlen = 0;

  /** capture %hhx will be treated as decimal, use %hx or %x for hex */
  while (sscanf(raw, "%hx[,]*", &tmp) == 1) {
    wdata[wlen] = tmp & 0xFF;
    wlen++;
    raw += 2;
    if (*raw == ',') { raw++; }
    if (*raw == ' ') { break; }
    if (*raw == 0) { break; }
  }
  ho_cli_writefln(parser, "");
  return true;
}

HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmds_i2c,
                            ho_cli_cmd_i2c_read,
                            "-r",
                            .flag = HO_CMD_MATCH_PREFIX | HO_CMD_BATCH) {
  if (sscanf(raw, "-r%d", &rlen) != 1) {
    ho_cli_error(parser, "Parse number to read failed");
    return false;
  }

  return true;
}

HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmds_i2c, ho_cli_cmd_i2c_transfer, "run") {
  if (!ho_i2c_transfer(cmd_i2c_bus,
                       cmd_i2c_addr,
                       wdata,
                       wlen,
                       rlen,
                       rdata,
                       HO_MS(10) * (wlen + rlen))) {
    ho_cli_error(parser, "Transfer failed");
    return false;
  }

  if (rlen) {
    ho_cli_ok(parser, "Read following:");
    for (ho_u32 i = 0; i < rlen; i++) {
      ho_cli_writef(parser, "%02X ", rdata[i]);
    }
    ho_cli_writefln(parser, "");
  }
  return true;
}
