/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_stack_process.h>
#include <ho/ho_debug.h>

/* IMPL --------------------------------------------------------------------- */
ho_cstring HO_COMP_STACKPROC = "StackProc";

/* ----------------------------------- Api ---------------------------------- */
bool ho_stackproc_forward(struct ho_stackproc *self) {
  if (self->_current == self->steps) { return false; }
  if (self->callback) {
    ho_event(HO_COMP_STACKPROC, HO_EVT_FORWARD, self);
    if (self->callback(HO_STACKPROC_ENTER(self->_current))) {
      self->_current++;
    }
  }
  return self->_current < self->steps;
}

bool ho_stackproc_backward(struct ho_stackproc *self) {
  if (self->_current == 0) { return false; }
  if (self->callback) {
    ho_event(HO_COMP_STACKPROC, HO_EVT_BACKWARD, self);
    if (self->callback(HO_STACKPROC_LEAVE(self->_current - 1))) {
      self->_current--;
    }
  }
  return self->_current > 0;
}
