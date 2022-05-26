/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "ho_event_recorder.h"
#include <ho/ho_init.h>

/* -------------------------------------------------------------------------- */
HO_EVENT_RECORDER_ANONYMOUS(.component = &HO_COMP_INIT_STAGE) {
  const struct ho_init_stage *stage = dbg->object;
  EventRecord4(EventID(EventLevelAPI, 0x01, 0x00),
               (uint32_t) stage,
               (uint32_t) stage->head,
               (uint32_t) stage->tail,
               NULL);
}
