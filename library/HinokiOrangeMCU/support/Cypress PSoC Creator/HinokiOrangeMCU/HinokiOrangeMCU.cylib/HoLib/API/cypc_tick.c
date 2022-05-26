/* SPDX-License-Identifier: MIT */
#include <ho/ho_init.h>
#include "project.h"

/* LINKER ------------------------------------------------------------------- */
void *HO_LINKER_CYPC_TICK;

/* TICK START --------------------------------------------------------------- */
HO_INIT_ANONYMOUS(ho_init_clock) {
  CySysTickStart();
  CySysTickSetCallback(0, ho_process_ticker);
}
