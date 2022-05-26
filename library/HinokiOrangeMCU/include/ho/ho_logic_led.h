/* SPDX-License-Identifier: MIT */
#ifndef ___HO_LOGIC_LED_H___
#define ___HO_LOGIC_LED_H___

/* INC ---------------------------------------------------------------------- */
#include <ho/ho_ticker.h>
#include <ho/ho_logic.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_logic_led;
/* -------------------------------------------------------------------------- */
static inline void ho_logic_led_set_step(struct ho_logic_led *, ho_u16);
static inline ho_u16 ho_logic_led_get_step(struct ho_logic_led *);
static inline void ho_logic_led_write(struct ho_logic_led *, ho_u16);
static inline ho_u16 ho_logic_led_read(struct ho_logic_led *);

/* -------------------------------------------------------------------------- */
#define HO_LOGIC_LED_INSTANCE(_led, _writer, _active, ...)                     \
  struct ho_logic_led _led = {                                                 \
    .led = _writer, .step = 25, .active = _active, ##__VA_ARGS__               \
  };                                                                           \
  HO_TICKER_ADD(__ho_logic_led_ticker_callback,                                \
                .name = #_led,                                                 \
                .flag = HO_ENABLE,                                             \
                .payload = &_led);

/**
 * @brief Logic LED definition
 */
struct ho_logic_led {
  struct ho_logic_writer *const led;
  ho_u16 step, brightness, _count;
  enum ho_logic active;
};
/* -------------------------------------------------------------------------- */
static inline void ho_logic_led_set_step(struct ho_logic_led *self,
                                         ho_u16 step) {
  self->step = step;
}
static inline ho_u16 ho_logic_led_get_step(struct ho_logic_led *self) {
  return self->step;
}
static inline void ho_logic_led_write(struct ho_logic_led *self,
                                      ho_u16 brightness) {
  self->brightness = brightness;
}
static inline ho_u16 ho_logic_led_read(struct ho_logic_led *self) {
  return self->brightness;
}
/* -------------------------------------------------------------------------- */
HO_TICKER_CALLBACK(__ho_logic_led_ticker_callback);

#endif /* ___HO_LOGIC_LED_H___ */
