/* SPDX-License-Identifier: MIT */
#ifndef ___HO_OS_H___
#define ___HO_OS_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_config_inc.h"
#include "ho_compiler.h"

/** TODO: Operating System support */
/* INTERFACE ---------------------------------------------------------------- */
#if defined(HO_CONFIG_WEAK)
extern ho_u32 HO_ENTER_CRITICAL(void) HO_WEAK;
extern void HO_LEAVE_CRITICAL(ho_u32) HO_WEAK;
extern void HO_YIELD(void) HO_WEAK;
#else /** by defined */

#if !defined(HO_ENTER_CRITICAL)
#define HO_ENTER_CRITICAL() (0)
#endif /* HO_ENTER_CRITICAL */

#if !defined(HO_LEAVE_CRITICAL)
#define HO_LEAVE_CRITICAL(...) (void) (__VA_ARGS__)
#endif /* HO_LEAVE_CRITICAL */

#if !defined(HO_YIELD)
#define HO_YIELD()
#endif /* HO_YIELD */

#endif /** interface */

/* API ---------------------------------------------------------------------- */
#define HO_CRITICAL_AREA()                                                     \
  for (ho_u32 HO_LOCAL_VAR() = ({                                              \
         ho_u32 t = HO_ENTER_CRITICAL();                                       \
         HO_LEAVE_CRITICAL(1);                                                 \
         (t);                                                                  \
       });                                                                     \
       HO_LOCAL_VAR() != 0xFFFFFFFF;                                           \
       HO_LEAVE_CRITICAL(HO_LOCAL_VAR()), HO_LOCAL_VAR() = 0xFFFFFFFF)

#endif /* ___HO_OS_H___ */
