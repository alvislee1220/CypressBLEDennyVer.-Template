/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_ringbuffer.h>

/* COMPNENT ----------------------------------------------------------------- */
ho_cstring HO_COMP_RINGBUFFER = "RingBuffer";
ho_cstring HO_EVT_DROP = "Drop";

/* BYTE READER -------------------------------------------------------------- */
static bool has(void *payload) { return !ho_rb_is_empty(payload); }
static bool get(void *payload, char *buf) {
  if (has(payload)) {
    *buf = ho_rb_pop(payload);
    return true;
  }
  return false;
}

struct ho_byte_reader_interface ho_byte_reader_ringbuffer = {
  .get = get,
  .has = has,
};
