/* SPDX-License-Identifier: MIT */
#ifndef ___HO_UPTIME_H___
#define ___HO_UPTIME_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"

/* DECLARE ------------------------------------------------------------------ */
extern struct __ho_uptime ho_uptime;

/**
 * @brief Uptime
 */
struct __ho_uptime {
  ho_u32 day;
  ho_u32 hour;
  ho_u32 min;
  ho_u32 sec;
  ho_u32 msec;
};

/* SHOW --------------------------------------------------------------------- */
#define HO_UPTIME_LOGGER(_period)                                              \
  HO_ROUTINE_ANONYMOUS() {                                                     \
    HO_ROUTINE_INTERVAL(_period);                                              \
    ho_log("Uptime: %dd %d:%d:%d ::%d",                                        \
           ho_uptime.day,                                                      \
           ho_uptime.hour,                                                     \
           ho_uptime.min,                                                      \
           ho_uptime.sec,                                                      \
           ho_uptime.msec);                                                    \
  }

#endif /* ___HO_UPTIME_H___ */
