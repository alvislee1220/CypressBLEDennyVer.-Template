/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "`$INSTANCE_NAME`_cypc_i2c.h"
#include "`$I2C_INSTANCE`.h"
#include "`$I2C_INSTANCE`_I2C.h"

/* START -------------------------------------------------------------------- */
#include <ho/ho_init.h>
HO_INIT_ANONYMOUS(ho_init_peripheral) { `$I2C_INSTANCE`_Start(); }

/* INTERFACE ---------------------------------------------------------------- */
#include <ho/peripheral/ho_i2c.h>

static bool ack(void *self HO_UNUSED, ho_u16 addr, ho_u32 timeout) {
  bool res = `$I2C_INSTANCE`_I2CMasterSendStart(
               addr, `$I2C_INSTANCE`_I2C_WRITE_XFER_MODE, timeout) ==
             `$I2C_INSTANCE`_I2C_MSTR_NO_ERROR;
  `$I2C_INSTANCE`_I2CMasterSendStop(timeout);

  return res;
}

static bool transfer(void *self HO_UNUSED,
                     ho_u16 addr,
                     ho_u8 *wd,
                     ho_u32 wl,
                     ho_u32 rl,
                     ho_u8 *rd,
                     ho_u32 timeout) {
  if (wl) {
    (void)`$I2C_INSTANCE`_I2CMasterClearStatus();
    if (`$I2C_INSTANCE`_I2CMasterWriteBuf(
          addr,
          wd,
          wl,
          rl >
              0 ?`$I2C_INSTANCE`_I2C_MODE_NO_STOP :`$I2C_INSTANCE`_I2C_MODE_COMPLETE_XFER) != `$I2C_INSTANCE`_I2C_MSTR_NO_ERROR) {
      return false;
    }
    if (!HO_WAIT(
          ho_have_all(`$I2C_INSTANCE`_I2CMasterStatus(), `$I2C_INSTANCE`_I2C_MSTAT_WR_CMPLT),
          timeout)) {
      return false;
    }
    if (`$I2C_INSTANCE`_I2CMasterGetWriteBufSize() != wl) { return false; }
  }
  if (rl) {
    (void)`$I2C_INSTANCE`_I2CMasterClearStatus();
    if (`$I2C_INSTANCE`_I2CMasterReadBuf(
          addr,
          rd,
          rl,
          (wl >
               0 ?`$I2C_INSTANCE`_I2C_MODE_REPEAT_START :
                   0) |`$I2C_INSTANCE`_I2C_MODE_COMPLETE_XFER) != `$I2C_INSTANCE`_I2C_MSTR_NO_ERROR) {
      return false;
    }
    if (!HO_WAIT(
          ho_have_all(`$I2C_INSTANCE`_I2CMasterStatus(), `$I2C_INSTANCE`_I2C_MSTAT_RD_CMPLT),
          timeout)) {
      return false;
    }
    if (`$I2C_INSTANCE`_I2CMasterGetReadBufSize() != rl) { return false; }
  }

  (void)`$I2C_INSTANCE`_I2CMasterClearStatus();
  return true;
}

static ho_u32 get_state(void *self HO_UNUSED) {
  return `$I2C_INSTANCE`_I2CMasterStatus();
}

static bool is_ready(void *self HO_UNUSED) {
  return `$I2C_INSTANCE`_I2CMasterStatus() == `$I2C_INSTANCE`_I2C_MSTAT_CLEAR;
}

static struct ho_i2c_interface interface = {
  .ack = ack, .get_state = get_state, .is_ready = is_ready, .transfer = transfer
};

struct ho_i2c_bus `$I2C_INSTANCE`_bus = { .interface = &interface };

/* CLI ---------------------------------------------------------------------- */
#include <ho/ho_cli.h>
HO_I2C_CLI_CMD_INSTANCE(`$I2C_INSTANCE`_cmd,
                        HO_TO_STRING(`$I2C_INSTANCE`),
                        &`$I2C_INSTANCE`_bus);
