/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_os.h>

#if defined(HO_CONFIG_WEAK)
HO_WEAK ho_u32 HO_ENTER_CRITICAL(void) { return 0; }
HO_WEAK void HO_LEAVE_CRITICAL(ho_u32 _1 HO_UNUSED) {}
HO_WEAK void HO_YIELD(void){};

#endif /** weak */
