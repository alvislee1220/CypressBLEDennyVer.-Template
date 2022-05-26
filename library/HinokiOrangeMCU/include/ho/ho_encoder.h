/* SPDX-License-Identifier: MIT */
#ifndef ___HO_ENCODER_H___
#define ___HO_ENCODER_H___

/* INC ---------------------------------------------------------------------- */
#include <ho/ho_logic.h>
#include <ho/ho_routine.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_encoder;
/* -------------------------------------------------------------------------- */
static inline ho_i32 ho_encoder_read(struct ho_encoder *);
/* -------------------------------------------------------------------------- */
#define HO_ENCODER_INSTANCE(_enc, _cw, _ccw, _active, ...)                     \
  struct ho_encoder _enc = {                                                   \
    .cw = _cw, .ccw = _ccw, .active = _active, ##__VA_ARGS__                   \
  };                                                                           \
  HO_ROUTINE_ADD(                                                              \
    __ho_encoder_routine_callback, .name = #_enc, .payload = &_enc);

/**
 * @brief Encoder definition
 */
struct ho_encoder {
  volatile ho_i32 value;
  enum ho_logic active;
  struct ho_logic_reader *const cw, *const ccw;
  ho_flag flag;
};
/* -------------------------------------------------------------------------- */
HO_ROUTINE_CALLBACK(__ho_encoder_routine_callback);
/* -------------------------------------------------------------------------- */
static inline ho_i32 ho_encoder_read(struct ho_encoder *self) {
  ho_i32 v = self->value;
  HO_CRITICAL_AREA() { self->value = 0; }
  return v;
}

#endif /* ___HO_ENCODER_H___ */
