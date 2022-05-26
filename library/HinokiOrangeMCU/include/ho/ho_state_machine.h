/* SPDX-License-Identifier: MIT */
#ifndef ___HO_STATE_MACHINE_H___
#define ___HO_STATE_MACHINE_H___
/* INC ---------------------------------------------------------------------- */
#include "ho_sub_process.h"

/* DECLARE ------------------------------------------------------------------ */
struct ho_state_machine; /*!< state machine */
/* -------------------------------- Component ------------------------------- */
extern ho_cstring HO_COMP_STATE_MACHINE;
/* ----------------------------------- Api ---------------------------------- */
bool ho_state_machine_change(struct ho_state_machine *, struct ho_subproc *);

/**
 * @brief State Machine
 */
struct ho_state_machine {
  ho_cstring name;
  struct ho_subproc *state;
};

/* DEBUG -------------------------------------------------------------------- */
#define HO_STATE_MACHINE_DEFAULT_LOGGER()                                      \
  HO_DEBUG_FORMATTER_ANONYMOUS() {                                             \
    if (dbg->component != HO_COMP_STATE_MACHINE) { return 0; }                 \
    const struct ho_state_machine *s = dbg->object;                            \
    const struct ho_subproc *n = dbg->data[0];                                 \
    if (dbg->event == HO_EVT_START) {                                          \
      HO_DEBUG_FORMATTER_PRINTF("[%s] -> %s", s->name, n->name);               \
    } else if (dbg->event == HO_EVT_STOP) {                                    \
      HO_DEBUG_FORMATTER_PRINTF("[%s] %s ->", s->name, s->state->name);        \
    } else if (dbg->event == HO_EVT_CHANGE) {                                  \
      HO_DEBUG_FORMATTER_PRINTF("[%s] %s ==> %s",                              \
                                s->name,                                       \
                                (s->state != NULL) ? s->state->name : "NULL",  \
                                (n != NULL) ? n->name : "NULL");               \
    }                                                                          \
    return 0;                                                                  \
  }

#endif /* ___HO_STATE_MACHINE_H___ */
