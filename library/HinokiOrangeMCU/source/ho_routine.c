/* SPDX-License-Identifier: MIT */
/* Inc ---------------------------------------------------------------------- */
#include <ho/ho_routine.h>

/* Impl --------------------------------------------------------------------- */
ho_cstring HO_COMP_ROUTINE = "Routine";
/* -------------------------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_routine, ho_routine);

/* ----------------------------------- Api ---------------------------------- */
bool ho_routine_enable(struct ho_routine *self) {
  if (ho_have_all(self->flag, HO_ENABLE)) { return false; }
  ho_event(HO_COMP_ROUTINE, HO_EVT_ENABLE, self);
  ho_mask(self->flag, HO_ENABLE);
  return true;
}

bool ho_routine_disable(struct ho_routine *self) {
  if (ho_none_of(self->flag, HO_ENABLE)) { return false; }
  ho_event(HO_COMP_ROUTINE, HO_EVT_DISABLE, self);
  ho_unmask(self->flag, HO_ENABLE);
  return true;
}
