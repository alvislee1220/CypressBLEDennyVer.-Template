/* SPDX-License-Identifier: MIT */
#include <ho/ho.h>
#include "project.h"

/* LINKER ------------------------------------------------------------------- */
void *HO_LINKER_CYPC_DEBUG;

/* PREVENT ------------------------------------------------------------------ */
HO_UNUSED static inline void _UartPutString(const char *_1 HO_UNUSED) {}

/* DEBUG -------------------------------------------------------------------- */
/* ---------------------------------- Uart ---------------------------------- */
#if /** uart */ `$DEBUG_METHOD` == 1
HO_DEBUGGER_DEFAULT_OUTPUT() { `$UART_INSTANCE`_UartPutString(data); }

HO_INIT_ANONYMOUS(ho_init_service) {
  `$UART_INSTANCE`_UartPutString(HO_STRING_CLEAR_TERMINAL);
}
#else
#error Not implemented
#endif

/* HARDFAULT ---------------------------------------------------------------- */
ho_cstring HO_COMP_EVT_OOM = "OutOfMemory";

void CyBoot_IntDefaultHandler_Exception_EntryCallback(void) {
  ho_hardfault_callback();
}

void CyBoot_IntDefaultHandler_Enomem_Exception_Callback(void) {
  ho_event(HO_COMP_EVT_OOM, HO_COMP_EVT_OOM, NULL);
  ho_hardfault_callback();
}
