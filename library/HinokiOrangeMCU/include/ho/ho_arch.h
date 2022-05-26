/* SPDX-License-Identifier: MIT */
#ifndef ___HO_ARCH_H___
#define ___HO_ARCH_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_config_inc.h"
#include "ho_compiler.h"

/* INTERFACE ---------------------------------------------------------------- */
#if defined(HO_CONFIG_WEAK)
extern ho_u32 HO_MASK_IRQ(void) HO_WEAK;
extern void HO_UNMASK_IRQ(ho_u32) HO_WEAK;
#else /** by defined */

#if !defined(HO_MASK_IRQ)
#define HO_MASK_IRQ() (0)
#endif /* HO_MASK_IRQ */

#if !defined(HO_UNMASK_IRQ)
#define HO_UNMASK_IRQ(...) (void) (__VA_ARGS__)
#endif /* HO_UNMASK_IRQ */

#endif /** interface */

/* API ---------------------------------------------------------------------- */
#define HO_IRQ_AREA()                                                          \
  for (ho_u32 HO_LOCAL_VAR() = ({                                              \
         ho_u32 t = HO_MASK_IRQ();                                             \
         HO_UNMASK_IRQ(1);                                                     \
         (t);                                                                  \
       });                                                                     \
       HO_LOCAL_VAR() != 0xFFFFFFFF;                                           \
       HO_UNMASK_IRQ(HO_LOCAL_VAR()), HO_LOCAL_VAR() = 0xFFFFFFFF)

#endif /* ___HO_ARCH_H___ */
