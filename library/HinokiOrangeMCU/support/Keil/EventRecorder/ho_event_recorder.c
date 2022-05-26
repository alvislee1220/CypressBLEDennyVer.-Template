/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "ho_event_recorder.h"
#include <ho/ho_time.h>
#include <ho/ho_list.h>
#include <ho/ho_init.h>

/* MESSAGE ------------------------------------------------------------------ */
HO_EVENT_RECORDER_ANONYMOUS(.event = &HO_COMP_EVT_MESSAGE) {
  static uint32_t index = 0U;
  static uint8_t buffer[8];

  char *msg = (char *) dbg->data;
  ho_u32 len = dbg->len;
  while (len--) {
    buffer[index++] = (uint8_t) *msg;
    if ((index == 8U) || (*msg == '\n')) {
      EventRecordData(EventID(EventLevelOp, 0xFE, 0x00), buffer, index);
      index = 0U;
    }
    msg++;
  }
}

/* DEBUGGER PROCESS --------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_event_recorder_process, ho_event_recorder_process);
/* -------------------------------------------------------------------------- */
static void process(struct ho_debugger *self, struct ho_debug_desc *dbg) {
  HO_LIST_FOREACH(ho_event_recorder_process, evtproc) {
    if (((*evtproc)->component != NULL) &&
        (*((*evtproc)->component) != dbg->component)) {
      continue;
    }
    if (((*evtproc)->event != NULL) && (*((*evtproc)->event) != dbg->event)) {
      continue;
    }
    if ((*evtproc)->callback) { (*evtproc)->callback(dbg); }
    return;
  }
  EventRecord4(EventID(EventLevelDetail, 0x00, 0x00),
               (uint32_t) dbg->component,
               (uint32_t) dbg->event,
               (uint32_t) dbg->object,
               NULL);
}

/* TIMER -------------------------------------------------------------------- */
uint32_t EventRecorderTimerSetup(void) { return 1; }
uint32_t EventRecorderTimerGetFreq(void) { return 1000; }
uint32_t EventRecorderTimerGetCount(void) { return ho_get_tick(); }

/* INIT --------------------------------------------------------------------- */
HO_INIT_ANONYMOUS(ho_init_clock) {
  EventRecorderInitialize(EventRecordAll, 1U);
  EventRecorderStart();
}

/* ADD DEBUGGER ------------------------------------------------------------- */
HO_LIST_ADD(ho_debugger, (&((struct ho_debugger){ .process = process })));
