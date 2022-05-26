/* SPDX-License-Identifier: MIT */

#if `$INC_CHK` == 0
#error Include Path is not added yet.
#endif /** INC_CHK */

#if `$INLINE_CHK` == 0
#warning Optimize inline function is not configured yet.
#endif /** INLINE_CHK */

#if `$LD_CHK` == 0
#error Linker Script is not configured
#endif /** LD_CHK */

#if `$LD_CFG_CHK` == 0
#error Linker Command option is not added yet.
#endif /** LD_CFG_CHK */

#if `$API_CHK` == 0
#error cyapicallbacks.h need include <ho/cypc/ho_cyapicallbacks.h>
#endif /** API_CHK */
