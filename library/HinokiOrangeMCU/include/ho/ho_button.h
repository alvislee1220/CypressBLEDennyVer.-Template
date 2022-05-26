/* SPDX-License-Identifier: MIT */
#ifndef ___HO_BUTTON_H___
#define ___HO_BUTTON_H___
/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_logic.h"
#include "ho_routine.h"
#include <stdbool.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_btn;
/* COMPONENT ---------------------------------------------------------------- */
extern ho_cstring HO_COMP_BUTTON;
/* FLAG --------------------------------------------------------------------- */
#define HO_BTN_PRESS (1U << 0)
#define HO_BTN_RELEASE (1U << 1)
#define HO_BTN_CLICK_COUNT (1U << 2)
#define HO_BTN_LOCK (1U << 8)
/* API ---------------------------------------------------------------------- */
bool ho_btn_enable(struct ho_btn *);
bool ho_btn_disable(struct ho_btn *);
static inline void ho_btn_lock(struct ho_btn *);
static inline void ho_btn_unlock(struct ho_btn *);
static inline bool ho_btn_is_holding(struct ho_btn *);
static inline ho_u32 ho_btn_get_holds(struct ho_btn *);
static inline ho_u32 ho_btn_get_clicks(struct ho_btn *);
static inline void ho_btn_reset(struct ho_btn *);
static inline ho_u32 ho_btn_get_repeats(struct ho_btn *);
static inline ho_u32 ho_btn_get_repeat_holds(struct ho_btn *);
static inline void ho_btn_count_repeat(struct ho_btn *);
/* REGISTER ----------------------------------------------------------------- */
#define HO_BTN_ADD(_btn)                                                       \
  HO_ROUTINE_ADD(__ho_btn_routine_callback, .name = #_btn, .payload = (_btn)); \
  HO_LIST_ADD(ho_btn, _btn);

/**
 * @brief Button
 */
struct ho_btn {
  struct ho_logic_reader *const input;
  ho_flag flag;
  const enum ho_logic active;
  struct __ho_btn_timestamp {
    ho_u32 pressed;
    ho_u32 released;
  } stamp;
  struct __ho_btn_click_count {
    ho_u32 timeout;
    ho_u32 times;
  } click;
  struct __ho_btn_repeat {
    ho_u32 ts;
    ho_u32 times;
  } repeat;
};
/* IMPL --------------------------------------------------------------------- */
void __ho_btn_routine_callback(struct ho_routine *);
/* -------------------------------------------------------------------------- */
static inline void ho_btn_lock(struct ho_btn *self) {
  ho_btn_reset(self);
  ho_mask(self->flag, HO_BTN_LOCK);
}
static inline void ho_btn_unlock(struct ho_btn *self) {
  ho_unmask(self->flag, HO_BTN_LOCK);
}
static inline bool ho_btn_is_holding(struct ho_btn *self) {
  return ho_has_any(self->flag, HO_BTN_PRESS);
}
static inline ho_u32 ho_btn_get_holds(struct ho_btn *self) {
  return ho_none_of(self->flag, HO_BTN_LOCK) ?
           (ho_get_tick() - self->stamp.pressed) :
           0;
}
static inline ho_u32 ho_btn_get_clicks(struct ho_btn *self) {
  return ho_none_of(self->flag, HO_BTN_LOCK) &&
             ho_has_any(self->flag, HO_BTN_CLICK_COUNT) ?
           self->click.times :
           0;
}
static inline void ho_btn_reset(struct ho_btn *self) {
  self->click.times = 0;
  self->repeat.times = 0;
  self->flag = (self->flag & HO_ENABLE);
}

static inline ho_u32 ho_btn_get_repeats(struct ho_btn *self) {
  return ho_none_of(self->flag, HO_BTN_LOCK) &&
             ho_has_any(self->flag, HO_BTN_PRESS) ?
           self->repeat.times :
           0;
}
static inline ho_u32 ho_btn_get_repeat_holds(struct ho_btn *self) {
  return ho_none_of(self->flag, HO_BTN_LOCK) &&
             ho_has_any(self->flag, HO_BTN_PRESS) ?
           (ho_get_tick() - self->repeat.ts) :
           0;
}

static inline void ho_btn_count_repeat(struct ho_btn *self) {
  self->repeat.times++;
  self->repeat.ts = ho_get_tick();
}

#endif /* ___HO_BUTTON_H___ */
