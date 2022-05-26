/* SPDX-License-Identifier: MIT */
#ifndef ___HO_CONFIG_INC_H___
#define ___HO_CONFIG_INC_H___

/* CONFIG ------------------------------------------------------------------- */
#if /** config by header file */ defined(HO_CONFIG_HEADER)
#include HO_CONFIG_HEADER
#elif /** config by weak links */ defined(HO_CONFIG_WEAK)
#else /** default */
#endif /** config */

#endif /* ___HO_CONFIG_INC_H___ */
