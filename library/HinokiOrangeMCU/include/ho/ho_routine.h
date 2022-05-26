/* SPDX-License-Identifier: MIT */
#ifndef ___HO_ROUTINE_H___
#define ___HO_ROUTINE_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_list.h"
#include "ho_debug.h"

/* Declare ------------------------------------------------------------------ */
struct ho_routine; /*!< routine */
/* -------------------------------------------------------------------------- */
extern ho_cstring HO_COMP_ROUTINE;
/* -------------------------------------------------------------------------- */
bool ho_routine_enable(struct ho_routine *);
bool ho_routine_disable(struct ho_routine *);
/* -------------------------------------------------------------------------- */
static inline void ho_process_routine(void);
/* -------------------------------------------------------------------------- */
#define HO_ROUTINE_CALLBACK(_name)                                             \
  void _name(struct ho_routine *routine HO_UNUSED)
#define HO_ROUTINE_STATIC(_var, ...)                                           \
  static HO_ROUTINE_CALLBACK(HO_LOCAL_VAR(callback));                          \
  static struct ho_routine _var = { .name = #_var,                             \
                                    .callback = HO_LOCAL_VAR(callback),        \
                                    ##__VA_ARGS__ };                           \
  HO_LIST_ADD(ho_routine, &_var);                                              \
  static HO_ROUTINE_CALLBACK(HO_LOCAL_VAR(callback))
#define HO_ROUTINE_ANONYMOUS(...)                                              \
  HO_ROUTINE_STATIC(HO_LOCAL_VAR(), .flag = HO_ENABLE, ##__VA_ARGS__)
/* -------------------------------------------------------------------------- */
#define HO_ROUTINE_ADD(_callback, ...)                                         \
  HO_LIST_ADD(ho_routine,                                                      \
              (&((struct ho_routine){ .callback = _callback,                   \
                                      .flag = HO_ENABLE,                       \
                                      .name = #_callback,                      \
                                      ##__VA_ARGS__ })))
/* -------------------------------------------------------------------------- */
#define HO_ROUTINE_INTERVAL(_ticks)                                            \
  static struct ho_cd HO_LOCAL_VAR() = { .is_act = true };                     \
  if (!ho_cd_is_done(&HO_LOCAL_VAR(), _ticks)) { return; }                     \
  ho_cd_recount(&HO_LOCAL_VAR());

/**
 * @brief Routine
 */
struct ho_routine {
  ho_cstring name;
  void (*const callback)(struct ho_routine *);
  ho_flag flag;
  ho_payload payload;
};
/* -------------------------------------------------------------------------- */
HO_LIST_DECLARE(struct ho_routine, ho_routine);
/* -------------------------------------------------------------------------- */
static inline void ho_process_routine(void) {
  HO_LIST_FOREACH(ho_routine, r) {
    if (ho_none_of((*r)->flag, HO_ENABLE)) { continue; }
    ho_event(HO_COMP_ROUTINE, HO_EVT_RUN, *r);
    (*r)->callback(*r);
  }
}

#endif /* ___HO_ROUTINE_H___ */
