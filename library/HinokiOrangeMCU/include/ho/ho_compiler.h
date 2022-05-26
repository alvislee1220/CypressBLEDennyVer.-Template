/* SPDX-License-Identifier: MIT */
#ifndef ___HO_COMPILER_H___
#define ___HO_COMPILER_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_config_inc.h"

/* Compiler ----------------------------------------------------------------- */
#if /** armcc */ defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
#define HO_GCC
#define HO_ARM
#elif /** armclang */ defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define HO_CLANG
#define HO_ARM
#elif /** clang */ defined(__clang__)
#define HO_CLANG
#elif /** gcc */ defined(__GNUC__)
#define HO_GCC
#endif /** compiler */

/* Warning ------------------------------------------------------------------ */
#if /** clang compatible */ defined(HO_CLANG)
#pragma clang diagnostic ignored "-Winitializer-overrides"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wextra-semi"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#pragma clang diagnostic ignored "-Wgnu-empty-initializer"
#pragma clang diagnostic ignored "-Wzero-length-array"
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
#pragma clang diagnostic ignored "-Wint-conversion"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
#endif /** warning */

/* Attribute ---------------------------------------------------------------- */
#if /** gcc compatible */ defined(HO_CLANG) || defined(HO_GCC)
#define HO_ROM(_section) __attribute__((used, section(".text.ho." _section)))
#define HO_RAM(_section) __attribute__((used, section(".data.ho." _section)))
#define HO_UNUSED __attribute__((unused))
#define HO_USED __attribute__((used))
#define HO_ALIGNED(_n) __attribute__((aligned(_n)))
#define HO_WEAK __attribute__((weak))
#define HO_NORETURN __attribute__((__noreturn__))
#endif

/* -------------------------------------------------------------------------- */
/* DEFAULT ------------------------------------------------------------------ */
#if !defined(HO_ROM)
#define HO_ROM(...)
#error ROM Section is necessary
#endif /* HO_ROM */

#if !defined(HO_RAM)
#define HO_RAM(...)
#error RAM Section is necessary
#endif /* HO_RAM */

#if !defined(HO_UNUSED)
#define HO_UNUSED
#endif /* HO_UNUSED */

#if !defined(HO_USED)
#define HO_USED
#endif /* HO_USED */

#if !defined(HO_ALIGNED)
#define HO_ALIGNED(...)
#endif /* HO_ALIGNED */

#if !defined(HO_WEAK)
#define HO_WEAK
#error Weak is necessary
#endif /* HO_WEAK */

#if !defined(HO_NORETURN)
#define HO_NORETURN
#endif /* HO_NORETURN */

#endif /* ___HO_COMPILER_H___ */
