/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_arch.h>

#if defined(HO_CONFIG_WEAK)
/* WEAK --------------------------------------------------------------------- */
HO_WEAK ho_u32 HO_MASK_IRQ(void) { return 0; }
HO_WEAK void HO_UNMASK_IRQ(ho_u32 _1 HO_UNUSED) {}
#endif /** weak link */
