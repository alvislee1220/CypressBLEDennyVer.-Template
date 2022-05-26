/* SPDX-License-Identifier: MIT */
#ifndef ___HO_STACK_PROCESS_H___
#define ___HO_STACK_PROCESS_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include <stdbool.h>

/* DECLARE ------------------------------------------------------------------ */
struct ho_stackproc; /*!< stack process */
/* -------------------------------- Component ------------------------------- */
extern ho_cstring HO_COMP_STACKPROC;
/* ----------------------------------- Api ---------------------------------- */
bool ho_stackproc_forward(struct ho_stackproc *);
bool ho_stackproc_backward(struct ho_stackproc *);
/* -------------------------------------------------------------------------- */
#define HO_STACKPROC_ENTER(_step) ((_step) | 0x00U)
#define HO_STACKPROC_LEAVE(_step) ((_step) | 0x80U)

/**
 * @brief Stack Process
 */
struct ho_stackproc {
  ho_cstring name;
  bool (*const callback)(ho_u32);
  const ho_u8 steps;
  ho_u8 _current;
};

#endif /* ___HO_STACK_PROCESS_H___ */
