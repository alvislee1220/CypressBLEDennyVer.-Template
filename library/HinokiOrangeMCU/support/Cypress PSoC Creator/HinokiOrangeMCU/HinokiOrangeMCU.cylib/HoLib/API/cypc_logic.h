/* SPDX-License-Identifier: MIT */
#ifndef ___CYPC_LOGIC_H___
#define ___CYPC_LOGIC_H___

/* INC ---------------------------------------------------------------------- */
#include <ho/ho_logic.h>
#include "project.h"

/* DECLARE ------------------------------------------------------------------ */
struct ho_cypc_pin {
  reg32 *ps;
  uint32_t mask;
  uint8_t shift;
};
/* API ---------------------------------------------------------------------- */
static inline enum ho_logic ho_cypc_pin_read(struct ho_cypc_pin *);
/* -------------------------------------------------------------------------- */
HO_LOGIC_READER_CALLBACK(cypc);
/* LOGIC -------------------------------------------------------------------- */
#define HO_CYPC_LOGIC_READER_MAKE(_pin)                                        \
  (struct ho_logic_reader) {                                                   \
    .name = #_pin,                                                             \
    HO_LOGIC_READER_CONF(                                                      \
      cypc,                                                                    \
      (&((struct ho_cypc_pin){                                                 \
        .ps = &_pin##_PS, .mask = _pin##_MASK, .shift = _pin##_SHIFT })))      \
  }

/* IMPL --------------------------------------------------------------------- */
static inline enum ho_logic ho_cypc_pin_read(struct ho_cypc_pin *self) {
  return (((*self->ps & self->mask) >> self->shift) > 0) ? HO_HIGH : HO_LOW;
}

#endif /* ___CYPC_LOGIC_H___ */
