/* SPDX-License-Identifier: MIT */
#ifndef ___HO_LOGIC_H___
#define ___HO_LOGIC_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_routine.h"

/* LOGIC -------------------------------------------------------------------- */
enum ho_logic { HO_LOW, HO_HIGH };

/* READER ------------------------------------------------------------------- */
struct ho_logic_reader; /*!< logic reader */
/* -------------------------------------------------------------------------- */
static inline enum ho_logic ho_logic_read(struct ho_logic_reader *);
/* -------------------------------------------------------------------------- */
#define HO_LOGIC_READER_CALLBACK(_name)                                        \
  enum ho_logic _name(void *payload HO_UNUSED)

/* WRITER ------------------------------------------------------------------- */
struct ho_logic_writer;
/* ----------------------------------- Api ---------------------------------- */
static inline void ho_logic_write(struct ho_logic_writer *, enum ho_logic);
/* -------------------------------------------------------------------------- */
#define HO_LOGIC_WRITER_CALLBACK(_name)                                        \
  void _name(void *payload HO_UNUSED, enum ho_logic logic)

/* INJECTABLE --------------------------------------------------------------- */
static inline void ho_logic_inject(struct ho_logic_reader *, enum ho_logic);
/* -------------------------------------------------------------------------- */
#define HO_LOGIC_READER_MAKE_INJECTABLE(_logic)                                \
  (struct ho_logic_reader) {                                                   \
    .read = __ho_logic_reader_injectable_callback,                             \
    .payload = (enum ho_logic[1]) {                                            \
      _logic                                                                   \
    }                                                                          \
  }

/* FILTER ------------------------------------------------------------------- */
static inline enum ho_logic ho_logic_filter_raw(struct ho_logic_reader *);
/* -------------------------------------------------------------------------- */
#define HO_LOGIC_READER_INSTANCE_FILTER(                                       \
  _name, _reader, _rising, _falling, ...)                                      \
  struct ho_logic_reader _name = (struct ho_logic_reader){                     \
    .read = __ho_logic_reader_filter_callback,                                 \
    .payload = (&(__HO_LOGIC_FILTER_MAKE(_reader, _rising, _falling)))         \
  };                                                                           \
  HO_ROUTINE_ADD(__ho_logic_filter_routine_callback,                           \
                 .payload = &_name,                                            \
                 .flag = HO_ENABLE,                                            \
                 .name = #_name);

/**
 * @brief Logic Reader
 */
struct ho_logic_reader {
  enum ho_logic (*const read)(void *);
  ho_payload payload;
};
/* -------------------------------------------------------------------------- */
static inline enum ho_logic ho_logic_read(struct ho_logic_reader *self) {
  return self->read(self->payload);
}

/**
 * @brief Writer
 */
struct ho_logic_writer {
  void (*const write)(void *, enum ho_logic);
  ho_payload payload;
};
/* ----------------------------------- Api ---------------------------------- */
static inline void ho_logic_write(struct ho_logic_writer *self,
                                  enum ho_logic logic) {
  self->write(self->payload, logic);
}

/**
 * @brief Logic Injectable
 */
extern HO_LOGIC_READER_CALLBACK(__ho_logic_reader_injectable_callback);
/* -------------------------------------------------------------------------- */
static inline void ho_logic_inject(struct ho_logic_reader *self,
                                   enum ho_logic logic) {
  *(enum ho_logic *) self->payload = logic;
}

/**
 * @brief Filter
 */
struct __ho_logic_filter {
  struct ho_logic_reader *const raw;
  enum ho_logic logic;
  struct __ho_logic_filter_rt {
    ho_u32 _ts;
    const ho_u32 rising; /*!< ticks to filter rising (low -> high) */
    const ho_u32 falling; /*!< ticks to filter falling (high -> low) */
  } filter;
};
/* -------------------------------------------------------------------------- */
#define __HO_LOGIC_FILTER_MAKE(_raw, _rising, _falling)                        \
  (struct __ho_logic_filter) {                                                 \
    .raw = _raw, .filter = {.rising = _rising, .falling = _falling }           \
  }
/* -------------------------------------------------------------------------- */
extern HO_LOGIC_READER_CALLBACK(__ho_logic_reader_filter_callback);
extern HO_ROUTINE_CALLBACK(__ho_logic_filter_routine_callback);
/* -------------------------------------------------------------------------- */
static inline enum ho_logic ho_logic_filter_raw(struct ho_logic_reader *self) {
  return ho_logic_read(((struct __ho_logic_filter *) self->payload)->raw);
}

#endif /* ___HO_LOGIC_H___ */
