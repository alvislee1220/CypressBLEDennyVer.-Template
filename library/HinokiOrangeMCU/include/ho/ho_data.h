/* SPDX-License-Identifier: MIT */
#ifndef ___HO_DATA_H___
#define ___HO_DATA_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"

/* DECLARE ------------------------------------------------------------------ */
struct ho_data;
/* -------------------------------------------------------------------------- */
#define HO_DATA_MAKE(_var, ...)                                                \
  { .data = _var, ##__VA_ARGS__ }
#define HO_DATA_MAKE_STEP(_var, _min, _max)                                    \
  HO_DATA_MAKE(                                                                \
    _var,                                                                      \
    (&((struct __ho_data_payload_step){ .min = (_min), .max = (_max) -1 })))
/* API ---------------------------------------------------------------------- */
#define ho_data_mark_changed(_data, ...)                                       \
  do {                                                                         \
    ho_mask((_data)->changed, HO_OVERRIDE(0xFFFFFFFF, ##__VA_ARGS__));         \
  } while (0)
#define ho_data_is_changed(_data, ...)                                         \
  (ho_has_any((_data)->changed, HO_OVERRIDE(0xFFFFFFFF, ##__VA_ARGS__)))
#define ho_data_clear_changed(_data, ...)                                      \
  do {                                                                         \
    ho_unmask((_data)->changed, HO_OVERRIDE(0xFFFFFFFF, ##__VA_ARGS__));       \
  } while (0)
#define ho_data_is_changed_rc(_data, ...)                                      \
  ({                                                                           \
    bool r = ho_data_is_changed(_data, ##__VA_ARGS__);                         \
    ho_data_clear_changed(_data, ##__VA_ARGS__);                               \
    (r);                                                                       \
  })
/* -------------------------------------------------------------------------- */
#define ho_data_ref_val(_data, _type, ...)                                     \
  ((__VA_ARGS__ _type *) ((_data)->data))
#define ho_data_read_val(_data, _type) (*(ho_data_ref_val(_data, _type, const)))
/* -------------------------------------------------------------------------- */
#define ho_data_write(_data, _value, ...)                                      \
  ({                                                                           \
    bool is_change = false;                                                    \
    if (ho_data_read_val(_data, typeof(_value)) != (_value)) {                 \
      HO_CRITICAL_AREA() {                                                     \
        *ho_data_ref_val(_data, typeof(_value)) = _value;                      \
        ho_data_mark_changed(_data, ##__VA_ARGS__);                            \
      }                                                                        \
      is_change = true;                                                        \
    }                                                                          \
    (is_change);                                                               \
  })
#define ho_data_write_ptr(_data, _ptr, _size, ...)                             \
  ({                                                                           \
    bool is_change = false;                                                    \
    if (memcmp((_data)->data, _ptr, _size) != 0) {                             \
      HO_CRITICAL_AREA() {                                                     \
        memcpy((_data)->data, _ptr, _size);                                    \
        ho_data_mark_changed(_data, ##__VA_ARGS__);                            \
      }                                                                        \
      is_change = true;                                                        \
    }                                                                          \
    (is_change);                                                               \
  })
#define ho_data_write_string(_data, _str, ...)                                 \
  ho_data_write_ptr(_data, _str, strlen(_str) + 1, ##__VA_ARGS__)
/* STEP --------------------------------------------------------------------- */
#define ho_data_step_up(_data, ...)                                            \
  do {                                                                         \
    if (ho_data_read_val(_data) < (__HO_DATA_STEP_PAYLOAD(_data)->max)) {      \
      ho_data_write(_data, (ho_data_read_val(_data) + 1));                     \
    } else if (__HO_DATA_STEP_OVER##__VA_ARGS__) {                             \
      ho_data_write(_data, (__HO_DATA_STEP_PAYLOAD(_data)->min));              \
    }                                                                          \
  } while (0)
#define ho_data_step_cycle_up(_var) ho_data_step_up(_var, CYCLE)
#define ho_data_step_down(_var, ...)                                           \
  do {                                                                         \
    if ((ho_data_read_val(_var)) > (__HO_DATA_STEP_PAYLOAD(_data)->min)) {     \
      ho_data_write(_var, (ho_data_read_val(_var) - 1));                       \
    } else if (__HO_DATA_STEP_OVER##__VA_ARGS__) {                             \
      ho_data_write(_var, (__HO_DATA_STEP_PAYLOAD(_data)->max));               \
    }                                                                          \
  } while (0)
#define ho_data_step_cycle_down(_var) ho_data_step_down(_var, CYCLE)

/**
 * @brief data
 */
struct ho_data {
  void *const data;
  ho_u32 changed;
  ho_payload payload;
};

/**
 * @brief Step payload for data
 */
struct __ho_data_payload_step {
  const ho_u32 min, max;
};
/* -------------------------------------------------------------------------- */
#define __HO_DATA_STEP_PAYLOAD(_data)                                          \
  ((struct __ho_data_payload_step *) ((_data)->payload))
/* -------------------------------------------------------------------------- */
#define __HO_DATA_STEP_OVER false
#define __HO_DATA_STEP_OVER_CYCLE true

#endif /* ___HO_DATA_H___ */
