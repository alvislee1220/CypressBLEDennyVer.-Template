/* SPDX-License-Identifier: MIT */
#ifndef ___HO_COUNTDOWN_H___
#define ___HO_COUNTDOWN_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_time.h"
#include <stdbool.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_cd; /*!< countdown */
/* API ---------------------------------------------------------------------- */
static inline void ho_cd_recount(struct ho_cd *);
static inline bool ho_cd_is_done(struct ho_cd *, ho_u32);
static inline void ho_cd_stop(struct ho_cd *);
static inline bool ho_cd_is_act(struct ho_cd *);

/**
 * @brief Countdown
 */
struct ho_cd {
  ho_u32 timestamp;
  bool is_act;
};
/* IMPL --------------------------------------------------------------------- */
static inline void ho_cd_recount(struct ho_cd *self) {
  self->timestamp = ho_get_tick();
  self->is_act = true;
}
static inline bool ho_cd_is_done(struct ho_cd *self, ho_u32 compare) {
  if (self->is_act && ho_is_ticks_up(self->timestamp, compare)) {
    self->is_act = false;
    return true;
  }
  return false;
}
static inline void ho_cd_stop(struct ho_cd *self) { self->is_act = false; }
static inline bool ho_cd_is_act(struct ho_cd *self) { return self->is_act; }

#endif /* ___HO_COUNTDOWN_H___ */
