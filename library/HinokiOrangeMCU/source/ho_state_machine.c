/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_state_machine.h>

/* IMPL --------------------------------------------------------------------- */
ho_cstring HO_COMP_STATE_MACHINE = "StateMachine";
/* ----------------------------------- Api ---------------------------------- */
bool ho_state_machine_change(struct ho_state_machine *self,
                             struct ho_subproc *new_state) {
  if (self->state == new_state) { return false; }

  ho_event(HO_COMP_STATE_MACHINE, HO_EVT_CHANGE, self, new_state);
  /** stop old */
  if (self->state) {
    ho_event(HO_COMP_STATE_MACHINE, HO_EVT_STOP, self);
    ho_subproc_stop(self->state);
  }
  /** start new */
  if (new_state) {
    ho_event(HO_COMP_STATE_MACHINE, HO_EVT_START, self, new_state);
    ho_subproc_start(new_state);
  }
  /** change */
  self->state = new_state;

  return true;
}
