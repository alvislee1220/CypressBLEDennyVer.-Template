/* SPDX-License-Identifier: MIT */
#ifndef ___HO_DATABASE_H___
#define ___HO_DATABASE_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_list.h"
#include <stdbool.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_db; /*!< database */
/* -------------------------------------------------------------------------- */
extern ho_cstring HO_COMP_DATABASE;
/* ----------------------------------- Api ---------------------------------- */
static inline void *ho_db_ptr(const struct ho_db *);
static inline ho_u32 ho_db_get_size(const struct ho_db *);
bool ho_db_valid(const struct ho_db *);
/* --------------------------------- Section -------------------------------- */
#define __HO_FOR_DB(_db, ...) HO_RAM("db." #_db __VA_ARGS__)
#define HO_FOR_DB(_db, ...)                                                    \
  __HO_FOR_DB(_db, ".data" __VA_ARGS__ "." HO_TO_STRING(__LINE__))
/* -------------------------------------------------------------------------- */
#define HO_DB_INSTANCE(_db, _version, ...)                                     \
  static ho_u32 HO_LIST_HEAD(_db) __HO_FOR_DB(_db, HO_ANSI_MIN) = 0;           \
  static ho_u32 HO_LIST_TAIL(_db) __HO_FOR_DB(_db, HO_ANSI_MAX) = 0;           \
  const struct ho_db _db = (struct ho_db) {                                    \
    .chead = (_version), .ctail = ~(_version), .head = &HO_LIST_HEAD(_db),     \
    .tail = &HO_LIST_TAIL(_db), .name = #_db, ##__VA_ARGS__                    \
  }

/**
 * @brief Database
 */
struct ho_db {
  ho_cstring name;
  const ho_u32 chead, ctail;
  ho_u32 *const head, *const tail;
};
/* ---------------------------------- Impl ---------------------------------- */
static inline void *ho_db_ptr(const struct ho_db *self) {
  return (void *) (self->head);
}
static inline ho_u32 ho_db_get_size(const struct ho_db *self) {
  return HO_ADDR_SIZE(self->head, self->tail);
}

#endif /* ___HO_DATABASE_H___ */
