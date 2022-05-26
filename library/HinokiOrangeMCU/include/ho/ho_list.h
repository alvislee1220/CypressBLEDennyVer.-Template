/* SPDX-License-Identifier: MIT */
#ifndef ___HO_LIST_H___
#define ___HO_LIST_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include <stdio.h>

/* ALIGN -------------------------------------------------------------------- */
#define HO_LIST_ALIGN_BEG "." HO_ANSI_MIN
#define HO_LIST_ALIGN_END "." HO_ANSI_MAX

/* Def ---------------------------------------------------------------------- */
#define HO_LIST_HEAD(_name) _name##_head
#define HO_LIST_TAIL(_name) _name##_tail

/* Api ---------------------------------------------------------------------- */
#define HO_LIST_DECLARE(_type, _list)                                          \
  extern _type *const HO_LIST_HEAD(_list);                                     \
  extern _type *const HO_LIST_TAIL(_list);

#define HO_LIST_INSTANCE(_type, _list)                                         \
  _type *const HO_LIST_HEAD(_list) HO_ROM(#_list HO_LIST_ALIGN_BEG) = NULL;    \
  _type *const HO_LIST_TAIL(_list) HO_ROM(#_list HO_LIST_ALIGN_END) = NULL;

#define HO_LIST_ADD(_list, _var, ...)                                          \
  static typeof(*(_var)) *const HO_LINE_VAR(_list##_)                          \
    HO_ROM(#_list ".data." __VA_ARGS__ "." HO_TO_STRING(__LINE__)) HO_USED =   \
      _var

#define HO_LIST_FOREACH(_list, _var)                                           \
  _HO_LIST_FOREACH(&HO_LIST_HEAD(_list), &HO_LIST_TAIL(_list), _var)

/**
 * @brief
 * @param _head_ptr list_type**
 * @param _tail_ptr list_type**
 */
#define _HO_LIST_FOREACH(_head_ptr, _tail_ptr, _var)                           \
  for (typeof(**(_head_ptr)) *const *_var = (_head_ptr) + 1;                   \
       (_var < (_tail_ptr));                                                   \
       _var++)

/* Struct ------------------------------------------------------------------- */
#define HO_LIST_STRUCT(_type, ...)                                             \
  struct __VA_ARGS__ {                                                         \
    _type *const *const head;                                                  \
    _type *const *const tail;                                                  \
  }

#define HO_LIST_STRUCT_MAKE(_list)                                             \
  { .head = &HO_LIST_HEAD(_list), .tail = &HO_LIST_TAIL(_list) }

#define HO_LIST_STRUCT_FOREACH(_struct, _var)                                  \
  _HO_LIST_FOREACH(((_struct)->head), ((_struct)->tail), _var)

#endif /* ___HO_LIST_H___ */
