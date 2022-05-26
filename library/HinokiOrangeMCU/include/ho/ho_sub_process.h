/* SPDX-License-Identifier: MIT */
#ifndef ___HO_SUB_PROCESS_H___
#define ___HO_SUB_PROCESS_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_routine.h"
#include <stdbool.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_subproc; /*!< sub process */
/* ---------------------------------- Phase --------------------------------- */
typedef ho_cstring ho_subproc_phase;
#define HO_SUBPROC_PHASE_START HO_EVT_START
#define HO_SUBPROC_PHASE_STOP HO_EVT_STOP
#define HO_SUBPROC_PHASE_RUN HO_EVT_RUN
/* -------------------------------- Component ------------------------------- */
extern ho_cstring HO_COMP_SUBPROC;
/* ----------------------------------- Api ---------------------------------- */
bool ho_subproc_start(struct ho_subproc *);
bool ho_subproc_stop(struct ho_subproc *);
/* -------------------------------------------------------------------------- */
#define HO_SUBPROC_CALLBACK(_name)                                             \
  void _name(                                                                  \
    struct ho_subproc *subproc HO_UNUSED, ho_subproc_phase phase HO_UNUSED)
#define HO_SUBPROC_ADD_REF(_subproc, ...)                                      \
  HO_ROUTINE_ADD(                                                              \
    __ho_subproc_routine_callback, .payload = (_subproc), .name = #_subproc)
#define HO_SUBPROC_ADD(_name, ...)                                             \
  extern HO_SUBPROC_CALLBACK(_name##_subproc_callback);                        \
  struct ho_subproc _name = (struct ho_subproc){                               \
    .callback = _name##_subproc_callback,                                      \
    .name = #_name,                                                            \
    ##__VA_ARGS__,                                                             \
  };                                                                           \
  HO_SUBPROC_ADD_REF(&_name);                                                  \
  HO_SUBPROC_CALLBACK(_name##_subproc_callback)

/* IMPL --------------------------------------------------------------------- */
/**
 * @brief Subproc
 */
struct ho_subproc {
  ho_cstring name;
  void (*const callback)(struct ho_subproc *, ho_subproc_phase);
  ho_payload payload;
  ho_flag flag;
};
/* -------------------------------------------------------------------------- */
void __ho_subproc_routine_callback(struct ho_routine *);

/* DEBUG -------------------------------------------------------------------- */
/* DEBUG -------------------------------------------------------------------- */
#define HO_SUBPROC_DEFAULT_LOGGER()                                            \
  HO_DEBUG_FORMATTER_ANONYMOUS() {                                             \
    if (dbg->component != HO_COMP_SUBPROC) { return 0; }                       \
    const struct ho_subproc *s = dbg->object;                                  \
    if (dbg->event == HO_EVT_START) {                                          \
      HO_DEBUG_FORMATTER_PRINTF("[SubProc/Start] %s", s->name);                \
    } else if (dbg->event == HO_EVT_STOP) {                                    \
      HO_DEBUG_FORMATTER_PRINTF("[SubProc/Stop] %s", s->name);                 \
    }                                                                          \
    return 0;                                                                  \
  }

#endif /* ___HO_SUB_PROCESS_H___ */
