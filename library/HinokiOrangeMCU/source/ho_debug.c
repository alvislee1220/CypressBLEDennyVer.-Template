/* SPDX-License-Identifier: MIT */
/* Inc ---------------------------------------------------------------------- */
#include <ho/ho_debug.h>

/* Debugger ----------------------------------------------------------------- */
ho_cstring HO_COMP_EVT_MESSAGE = "Message";
ho_cstring HO_COMP_EVT_ASSERT = "Assert";
ho_cstring HO_COMP_EVT_HARDFAULT = "HardFault";

/* Common Events ------------------------------------------------------------ */
ho_cstring HO_EVT_INIT = "Init";
ho_cstring HO_EVT_DEINIT = "DeInit";
ho_cstring HO_EVT_RUN = "Run";
ho_cstring HO_EVT_FORWARD = "Forward";
ho_cstring HO_EVT_BACKWARD = "Backward";
ho_cstring HO_EVT_ENABLE = "Enable";
ho_cstring HO_EVT_DISABLE = "Disable";
ho_cstring HO_EVT_START = "Start";
ho_cstring HO_EVT_STOP = "Stop";
ho_cstring HO_EVT_ADD = "Add";
ho_cstring HO_EVT_CHANGE = "Change";
ho_cstring HO_EVT_INVALID = "Invalid";
ho_cstring HO_EVT_FAILED = "Failed";
ho_cstring HO_EVT_TIMEOUT = "Timeout";

/* Weak --------------------------------------------------------------------- */
HO_WEAK HO_DEBUGGER_DEFAULT_OUTPUT() {}

/* Hard Fault --------------------------------------------------------------- */
void ho_hardfault_callback(void) {
  ho_event(HO_COMP_EVT_HARDFAULT, HO_COMP_EVT_HARDFAULT, NULL);
  for (;;) {} /** block system */
}

/* Debugger ----------------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_debugger, ho_debugger);

/* ********************************* Default ******************************** */

HO_LIST_INSTANCE(ho_debugger_formatter, ho_debugger_formatter);

/* Debugger ----------------------------------------------------------------- */
static void default_debugger(struct ho_debugger *self,
                             struct ho_debug_desc *dbg) {
  char buf[HO_DEBUG_MSG_LEN] = { 0 };
  char *ptr = NULL;
  ho_u32 len = 0;

  HO_LIST_FOREACH(ho_debugger_formatter, formatter) {
    ptr = buf;
    len = (*formatter)(dbg, &ptr);
    if (len == 0) { continue; }
    ho_debugger_default_output(ptr, len);
    if (ho_none_of(len, HO_STR_NO_NEWLINE)) {
      ho_debugger_default_output("\r\n", 2);
    }
  }
}
HO_LIST_ADD(ho_debugger,
            (&((struct ho_debugger){ .process = default_debugger })));

/* Formatter ---------------------------------------------------------------- */
HO_DEBUG_FORMATTER_ANONYMOUS() {
  static HO_CSTR_WITH_LEN() hf_msg =
    HO_CSTR_WITH_LEN_MAKE("!!! HardFault occured");

  if ((dbg->event == HO_COMP_EVT_MESSAGE) && (dbg->len)) {
    HO_DEBUG_FORMATTER_USE((char *) dbg->data, dbg->len);
  } else if ((dbg->event == HO_COMP_EVT_HARDFAULT) &&
             (dbg->component == HO_COMP_EVT_HARDFAULT)) {
    HO_DEBUG_FORMATTER_USE(hf_msg.str, hf_msg.len);
  }
  return 0;
}
