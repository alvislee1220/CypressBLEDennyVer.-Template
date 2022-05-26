/* SPDX-License-Identifier: MIT */

/* Inc ---------------------------------------------------------------------- */
#include <ho/ho_time.h>

/* Global ------------------------------------------------------------------- */
volatile ho_u32 __ho_ticks = 0;

/* Api ---------------------------------------------------------------------- */

/**
 * @brief Delay for ticks
 * @param       ticks           for delay
 */
void ho_delay(ho_u32 ticks) {
  ho_u32 stamp = ho_get_tick();
  do { HO_YIELD(); } while (!ho_is_ticks_up(stamp, ticks));
}

/* CLI ---------------------------------------------------------------------- */
#include <ho/ho_cli.h>
HO_CLI_CMD_INSTANCE_WITH_HANDLER(ho_cli_cmd_delay,
                                 "delay",
                                 .flag = HO_CMD_MATCH_PREFIX | HO_CMD_BATCH) {
  ho_u32 delay = 0;
  if (sscanf(raw, "delay%u", &delay) != 1) {
    ho_cli_error(parser, "parse delay in ms failed");
    return false;
  }
  ho_delay(HO_MS(delay));
  return true;
}
