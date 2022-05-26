/* SPDX-License-Identifier: MIT */
#ifndef ___HO_TYPEDEF_H___
#define ___HO_TYPEDEF_H___

/* BASIC TYPE --------------------------------------------------------------- */
typedef unsigned int ho_u32;
typedef unsigned short ho_u16;
typedef unsigned char ho_u8;
typedef int ho_i32;

/* ***************************** Flag, Modifier ***************************** */
/* Common Flag -------------------------------------------------------------- */
#define HO_ENABLE (1U << 31)

/* Length Of String --------------------------------------------------------- */
#define HO_STR_NO_NEWLINE (1U << 31)

/* ********************************* Helper ********************************* */
/* Linevar ------------------------------------------------------------------ */
#define _HO_LINE_VAR_1(_var, _line, ...) _var##_L##_line##_##__VA_ARGS__
#define _HO_LINE_VAR(_var, _line, ...)                                         \
  _HO_LINE_VAR_1(_var, _line, ##__VA_ARGS__)
#define HO_LINE_VAR(_var, ...) _HO_LINE_VAR(_var, __LINE__, ##__VA_ARGS__)
#define HO_LOCAL_VAR(...) HO_LINE_VAR(_ho_local, ##__VA_ARGS__)

/* To String ---------------------------------------------------------------- */
#define _HO_TO_STRING(_x) #_x
#define HO_TO_STRING(_x) _HO_TO_STRING(_x)

/* ********************************* Bitmask ******************************** */
#define ho_have_all(_flag, _mask) (((_flag) & (_mask)) == (_mask))
#define ho_has_any(_flag, _mask) (((_flag) & (_mask)) > 0)
#define ho_none_of(_flag, _mask) (((_flag) & (_mask)) == 0)
#define ho_mask(_flag, _mask)                                                  \
  do { (_flag) |= (_mask); } while (0)
#define ho_unmask(_flag, _mask)                                                \
  do { (_flag) &= ~(_mask); } while (0)
/* Bit ---------------------------------------------------------------------- */
#define ho_bit_isset(_flag, _bit) ho_has_any(_flag, (1U << bit))
#define ho_bit_set(_flag, _bit) ho_mask(_flag, (1U << bit))
#define ho_bit_reset(_flag, _bit) ho_mask(_flag, (1U << bit))

/* ******************************* Common Type ****************************** */
typedef const char *const ho_cstring; /*!< const string */
typedef void *const ho_payload; /*!< payload */
typedef ho_u32 ho_flag; /*!< flag */

/* Const String With Length ------------------------------------------------- */
#define HO_CSTR_WITH_LEN(...)                                                  \
  const struct __VA_ARGS__ {                                                   \
    char *const str;                                                           \
    ho_u32 len;                                                                \
  }
#define HO_CSTR_WITH_LEN_MAKE(_str)                                            \
  { .str = _str, .len = sizeof(_str) - 1 }

/* Override ----------------------------------------------------------------- */
#define HO_OVERRIDE(_default, ...)                                             \
  ({                                                                           \
    typeof(_default) t[] = { _default, ##__VA_ARGS__ };                        \
    (t[(sizeof(t) / sizeof(t[0])) - 1]);                                       \
  })

/* ADDR SIZE ---------------------------------------------------------------- */
#define HO_ADDR_SIZE(_beg, _end)                                               \
  ((ho_u32) (((const char *) (_end + 1)) - ((const char *) (_beg))))

/* STRING COMP -------------------------------------------------------------- */
#define HO_CSTRING_CMP(_input, _len, _str)                                     \
  (((_len) == (sizeof(_str) - 1)) && (memcmp(_input, _str, _len) == 0))

/* ***************************** Special String ***************************** */
#define HO_STRING_CLEAR_TERMINAL "\033[2J\033[H"
#define HO_ANSI_MIN "!"
#define HO_ANSI_MAX "~"

/* ****************************** Root Include ****************************** */
#include "ho_config_inc.h"
#include "ho_compiler.h"
#include "ho_arch.h"
#include "ho_os.h"

#endif /* ___HO_TYPEDEF_H___ */
