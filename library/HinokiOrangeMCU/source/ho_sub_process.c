/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_sub_process.h>

/* IMPL --------------------------------------------------------------------- */
ho_cstring HO_COMP_SUBPROC = "SubProc";
/* -------------------------------------------------------------------------- */
void __ho_subproc_routine_callback(struct ho_routine *self) {
  struct ho_subproc *subproc = self->payload;
  if (ho_none_of(subproc->flag, HO_ENABLE)) { return; }
  ho_event(HO_COMP_SUBPROC, HO_EVT_RUN, subproc);
  subproc->callback(subproc, HO_SUBPROC_PHASE_RUN);
}

/* ----------------------------------- Api ---------------------------------- */
bool ho_subproc_start(struct ho_subproc *self) {
  if (ho_has_any(self->flag, HO_ENABLE)) { return false; }
  ho_event(HO_COMP_SUBPROC, HO_EVT_START, self);
  self->callback(self, HO_SUBPROC_PHASE_START);
  ho_mask(self->flag, HO_ENABLE);
  return true;
}

bool ho_subproc_stop(struct ho_subproc *self) {
  if (ho_none_of(self->flag, HO_ENABLE)) { return false; }
  ho_event(HO_COMP_SUBPROC, HO_EVT_STOP, self);
  self->callback(self, HO_SUBPROC_PHASE_STOP);
  ho_unmask(self->flag, HO_ENABLE);
  return true;
}
