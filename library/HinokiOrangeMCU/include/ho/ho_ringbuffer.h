/* SPDX-License-Identifier: MIT */
#ifndef ___HO_RINGBUFFER_H___
#define ___HO_RINGBUFFER_H___
/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include <stdbool.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_rb; /*!< Ring buffer */
/* COMPONENT ---------------------------------------------------------------- */
extern ho_cstring HO_COMP_RINGBUFFER;
extern ho_cstring HO_EVT_DROP;
/* API ---------------------------------------------------------------------- */
static inline void ho_rb_reset(struct ho_rb *);
static inline ho_u32 ho_rb_get_size(struct ho_rb *);
static inline ho_u32 ho_rb_get_buffered_length(struct ho_rb *);
static inline ho_u32 ho_rb_get_space(struct ho_rb *);
static inline bool ho_rb_is_full(struct ho_rb *);
static inline bool ho_rb_is_empty(struct ho_rb *);
static inline void ho_rb_push(struct ho_rb *, char);
static inline char ho_rb_pop(struct ho_rb *);
/* MAKE --------------------------------------------------------------------- */
#define HO_RB_MAKE(_size)                                                      \
  (struct ho_rb) {                                                             \
    .buffer =                                                                  \
      (char[((_size) & ((_size) -1)) == 0 ? _size : -1]){ [(_size) -1] = 0 },  \
    .read_cur = 0, .write_cur = 0, .mask = (_size) -1,                         \
  }

/* BYTE INTERFACE SUPPORT --------------------------------------------------- */
#include "ho_byte_interface.h"
extern struct ho_byte_reader_interface ho_byte_reader_ringbuffer;
/* -------------------------------------------------------------------------- */
#define HO_RB_BYTE_READER_MAKE(_rb)                                            \
  (struct ho_byte_reader) {                                                    \
    .interface = &ho_byte_reader_ringbuffer, .payload = _rb                    \
  }

/**
 * @brief Ring Buffer
 */
struct ho_rb {
  char *const buffer; /*!< buffer storage */
  volatile ho_u32 read_cur;
  volatile ho_u32 write_cur;
  const ho_u32 mask;
};
/* IMPL --------------------------------------------------------------------- */
static inline void ho_rb_reset(struct ho_rb *self) {
  HO_CRITICAL_AREA() {
    self->read_cur = 0;
    self->write_cur = 0;
  }
}
static inline ho_u32 ho_rb_get_size(struct ho_rb *self) { return self->mask; }
static inline ho_u32 ho_rb_get_buffered_length(struct ho_rb *self) {
  return ((self->write_cur - self->read_cur) & self->mask);
}
static inline ho_u32 ho_rb_get_space(struct ho_rb *self) {
  return self->mask - ho_rb_get_buffered_length(self);
}
static inline bool ho_rb_is_full(struct ho_rb *self) {
  return ho_rb_get_buffered_length(self) == self->mask;
}
static inline bool ho_rb_is_empty(struct ho_rb *self) {
  return ho_rb_get_buffered_length(self) == 0;
}
static inline void ho_rb_push(struct ho_rb *self, char data) {
  self->buffer[self->write_cur++ & self->mask] = data;
}
static inline char ho_rb_pop(struct ho_rb *self) {
  if (ho_rb_is_empty(self)) { return 0; }
  return self->buffer[self->read_cur++ & self->mask];
}

#endif /* ___HO_RINGBUFFER_H___ */
