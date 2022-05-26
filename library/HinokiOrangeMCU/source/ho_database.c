/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_database.h>
#include <ho/ho_debug.h>

/* IMPL --------------------------------------------------------------------- */
ho_cstring HO_COMP_DATABASE = "Database";

/* ----------------------------------- Api ----------------------------------
 */
bool ho_db_valid(const struct ho_db *self) {
  if (!ho_assert(self != NULL)) { return false; }
  do {
    if ((*self->head | *self->tail) != 0xFFFFFFFF) { break; }
    if (*self->head != self->chead) { break; }
    if (*self->tail != self->ctail) { break; }
    return true; /** valid */
  } while (0);
  /** invalid */
  ho_event(HO_COMP_DATABASE, HO_EVT_INVALID, self);
  HO_CRITICAL_AREA() {
    *self->head = self->chead;
    *self->tail = self->ctail;
  }
  return false;
}
