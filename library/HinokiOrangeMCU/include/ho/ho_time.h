/* SPDX-License-Identifier: MIT */
#ifndef ___HO_TIME_H___
#define ___HO_TIME_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include <stdbool.h>

/* Time Convert ------------------------------------------------------------- */

#define HO_MS(_ms) (_ms)
#define HO_SEC(_sec) ((_sec) *HO_MS(1000))
#define HO_MIN(_min) ((_min) *HO_SEC(60))
#define HO_HOUR(_hour) ((_hour) *HO_MIN(60))
#define HO_DAY(_day) ((_day) *HO_HOUR(24))

/* Global ------------------------------------------------------------------- */

extern volatile ho_u32 __ho_ticks;

/* Api ---------------------------------------------------------------------- */

static inline ho_u32 ho_get_tick(void);
static inline bool ho_is_ticks_up(ho_u32, ho_u32);
void ho_delay(ho_u32);

/* Macro -------------------------------------------------------------------- */

/**
 * @brief wait for condition or timeout
 * @param   _cond       condition       wait for
 * @param   _ticks      ho_u32    ticks for timeout
 * @return              true            condition success
 * @return              false           timeout
 */
#define HO_WAIT(_cond, _ticks)                                                 \
  ({                                                                           \
    ho_u32 stamp = ho_get_tick();                                              \
    bool is_ok = true;                                                         \
    while (!(_cond)) {                                                         \
      if (ho_is_ticks_up(stamp, _ticks)) {                                     \
        is_ok = false;                                                         \
        break;                                                                 \
      }                                                                        \
      HO_YIELD();                                                              \
    }                                                                          \
    (is_ok);                                                                   \
  })

/* Implement ---------------------------------------------------------------- */
/**
 * @brief Get current system tick
 * @return      ho_u32    system tick
 */
static inline ho_u32 ho_get_tick(void) { return __ho_ticks; }

/**
 * @brief Compare current and old timestamp, check is up to specific ticks
 * @param       stamp           old timestmap
 * @param       compare         compared ticks
 * @return      true            up
 * @return      false           not yet
 */
static inline bool ho_is_ticks_up(ho_u32 stamp, ho_u32 compare) {
  return (ho_get_tick() - stamp) >= compare;
}

/* CLI ---------------------------------------------------------------------- */
extern struct ho_cli_cmd ho_cli_cmd_delay;

#endif /* ___HO_TIME_H___ */
