/* SPDX-License-Identifier: MIT */
#ifndef ___HO_EVENT_RECORDER_H___
#define ___HO_EVENT_RECORDER_H___

/* INC ---------------------------------------------------------------------- */
#include <ho/ho_debug.h>
#include "EventRecorder.h"

/* DECLARE ------------------------------------------------------------------ */
struct ho_event_recorder_process;
/* -------------------------------------------------------------------------- */
#define HO_EVENT_RECORDER_ANONYMOUS(...)                                       \
  static void HO_VAR_LOCAL(struct ho_debug_desc *);                            \
  HO_LIST_ADD(ho_event_recorder_process,                                       \
              (&((struct ho_event_recorder_process){ .callback = HO_VAR_LOCAL, \
                                                     ##__VA_ARGS__ })));       \
  static void HO_VAR_LOCAL(struct ho_debug_desc *dbg)

/**
 * @brief Process
 */
struct ho_event_recorder_process {
  ho_cstring *component;
  ho_cstring *event;
  void (*const callback)(struct ho_debug_desc *);
};

#endif /* ___HO_EVENT_RECORDER_H___ */
