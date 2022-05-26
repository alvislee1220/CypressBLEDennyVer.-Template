/* SPDX-License-Identifier: MIT */
#ifndef ___HO_TICKER_H___
#define ___HO_TICKER_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_routine.h"

/* Declare ------------------------------------------------------------------ */
#define ho_ticker ho_ticker /*!< [list] fake list indicate */
/* -------------------------------------------------------------------------- */
static inline void ho_process_ticker(void);
/* -------------------------------------------------------------------------- */
#define HO_TICKER_CALLBACK(_name)                                              \
  void _name(struct ho_routine *ticker HO_UNUSED)
#define HO_TICKER_STATIC(_var, ...)                                            \
  static HO_TICKER_CALLBACK(HO_LOCAL_VAR(callback));                           \
  static struct ho_routine _var = { .name = #_var,                             \
                                    .callback = HO_LOCAL_VAR(callback),        \
                                    ##__VA_ARGS__ };                           \
  HO_LIST_ADD(ho_ticker, &_var);                                               \
  static HO_TICKER_CALLBACK(HO_LOCAL_VAR(callback))
#define HO_TICKER_ANONYMOUS(...)                                               \
  HO_TICKER_STATIC(HO_LOCAL_VAR(), .flag = HO_ENABLE, ##__VA_ARGS__)
/* -------------------------------------------------------------------------- */
#define HO_TICKER_ADD(_callback, ...)                                          \
  HO_LIST_ADD(ho_ticker,                                                       \
              (&((struct ho_routine){ .callback = _callback,                   \
                                      .flag = HO_ENABLE,                       \
                                      .name = #_callback,                      \
                                      ##__VA_ARGS__ })))
/* ------------------------------------------------------------------------ */
#define HO_TICKER_INTERVAL(_ticks)                                             \
  do {                                                                         \
    static ho_u32 HO_LOCAL_VAR() = 0;                                          \
    if (HO_LOCAL_VAR() < (_ticks)) {                                           \
      HO_LOCAL_VAR()++;                                                        \
      return;                                                                  \
    }                                                                          \
    HO_LOCAL_VAR() = 0;                                                        \
  } while (0)

/* Impl --------------------------------------------------------------------- */
HO_LIST_DECLARE(struct ho_routine, ho_ticker);
/* -------------------------------------------------------------------------- */
static inline void ho_process_ticker(void) {
  __ho_ticks++;
  HO_LIST_FOREACH(ho_ticker, t) {
    if (ho_none_of((*t)->flag, HO_ENABLE)) { continue; }
    (*t)->callback(*t);
  }
}

#endif /* ___HO_TICKER_H___ */
