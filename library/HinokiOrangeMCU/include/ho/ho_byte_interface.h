/* SPDX-License-Identifier: MIT */
#ifndef ___HO_BYTE_INTERFACE_H___
#define ___HO_BYTE_INTERFACE_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include <stdbool.h>

/* READER ------------------------------------------------------------------- */
struct ho_byte_reader; /*!< byte reader */
struct ho_byte_reader_interface; /*!< api interface for byte reader */
static inline bool ho_byte_reader_has(struct ho_byte_reader *);
static inline bool ho_byte_reader_get(struct ho_byte_reader *, char *);
/* WRITER ------------------------------------------------------------------- */
struct ho_byte_writer; /*!< byte writer */
struct ho_byte_writer_interface; /*!< api interface for byte writer */
static inline bool ho_byte_writer_put(struct ho_byte_writer *, char);
static inline ho_u32
ho_byte_writer_write(struct ho_byte_writer *, const char *, ho_u32);
/* -------------------------------------------------------------------------- */
#define HO_BYTE_WRITER_PUT_CALLBACK(_name)                                     \
  bool _name(void *payload HO_UNUSED, char byte HO_UNUSED)
#define HO_BYTE_WRITER_WRITE_CALLBACK(_name)                                   \
  ho_u32 _name(                                                                \
    void *payload HO_UNUSED, const char *data HO_UNUSED, ho_u32 len HO_UNUSED)
/**
 * @brief Reader Interface
 */
struct ho_byte_reader_interface {
  bool (*const has)(void *);
  bool (*const get)(void *, char *);
};

/**
 * @brief Byte Reader
 */
struct ho_byte_reader {
  const struct ho_byte_reader_interface *const interface;
  ho_payload payload;
};
/* IMPL --------------------------------------------------------------------- */
static inline bool ho_byte_reader_has(struct ho_byte_reader *self) {
  return self->interface->has(self->payload);
}
static inline bool ho_byte_reader_get(struct ho_byte_reader *self, char *buf) {
  return self->interface->get(self->payload, buf);
}

/**
 * @brief Writer Interface
 */
struct ho_byte_writer_interface {
  bool (*const put)(void *, char);
  ho_u32 (*const write)(void *, const char *, ho_u32);
};

/**
 * @brief Writer
 */
struct ho_byte_writer {
  const struct ho_byte_writer_interface *const interface;
  ho_payload payload;
};
/* IMPL --------------------------------------------------------------------- */
static inline bool ho_byte_writer_put(struct ho_byte_writer *self, char byte) {
  return self->interface->put(self->payload, byte);
}
static inline ho_u32 ho_byte_writer_write(struct ho_byte_writer *self,
                                          const char *data,
                                          ho_u32 len) {
  return self->interface->write(self->payload, data, len);
}

#endif /* ___HO_BYTE_INTERFACE_H___ */
