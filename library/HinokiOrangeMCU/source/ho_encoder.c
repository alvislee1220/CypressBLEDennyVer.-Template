/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_encoder.h>

/* FLAG --------------------------------------------------------------------- */
#define STX (1U << 0)
#define MID (1U << 1)
#define CW (1U << 2)
#define CCW (1U << 3)

/* IMPL --------------------------------------------------------------------- */
HO_ROUTINE_CALLBACK(__ho_encoder_routine_callback) {
  struct ho_encoder *enc = routine->payload;
  ho_u32 state = 0;

  /** state */
  state |= (ho_u32) (ho_logic_read(enc->ccw) == enc->active);
  state <<= 1;
  state |= (ho_u32) (ho_logic_read(enc->cw) == enc->active);

  /** case */
  switch (state) {
  case 0 /** STX */: enc->flag = STX; break;
  case 1 /** CW, !CCW */: ho_mask(enc->flag, CCW); break;
  case 2 /** !CW, CCW */: ho_mask(enc->flag, CW); break;
  case 3 /** CW, CCW */:
    if (ho_have_all(enc->flag, STX | CW)) {
      HO_CRITICAL_AREA() { enc->value++; }
    } else if (ho_have_all(enc->flag, STX | CCW)) {
      HO_CRITICAL_AREA() { enc->value--; }
    }
    enc->flag = 0;
    break;
  default: ho_assert(false); return;
  }
}
