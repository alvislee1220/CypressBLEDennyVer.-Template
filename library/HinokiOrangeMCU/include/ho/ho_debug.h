/* SPDX-License-Identifier: MIT */
#ifndef ___HO_DEBUG_H___
#define ___HO_DEBUG_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_list.h"
#include "ho_time.h"
#include <stdio.h>

/* **************************** Component & Event *************************** */
/* Debugger ----------------------------------------------------------------- */
extern ho_cstring HO_COMP_EVT_MESSAGE;
extern ho_cstring HO_COMP_EVT_ASSERT;
extern ho_cstring HO_COMP_EVT_HARDFAULT;

/* Common Events ------------------------------------------------------------ */
extern ho_cstring HO_EVT_INIT;
extern ho_cstring HO_EVT_DEINIT;
extern ho_cstring HO_EVT_RUN;
extern ho_cstring HO_EVT_FORWARD;
extern ho_cstring HO_EVT_BACKWARD;
extern ho_cstring HO_EVT_ENABLE;
extern ho_cstring HO_EVT_DISABLE;
extern ho_cstring HO_EVT_START;
extern ho_cstring HO_EVT_STOP;
extern ho_cstring HO_EVT_ADD;
extern ho_cstring HO_EVT_CHANGE;
extern ho_cstring HO_EVT_INVALID;
extern ho_cstring HO_EVT_FAILED;
extern ho_cstring HO_EVT_TIMEOUT;

/* Declare ------------------------------------------------------------------ */
struct ho_debug_desc; /*!< debug description */
struct ho_debugger; /*!< [list] debugger to process @p ho_debug_desc */
/*!< [list] debugger formatter for default output */
typedef ho_u32(ho_debugger_formatter)(struct ho_debug_desc *, char **);

/* Debug -------------------------------------------------------------------- */
#define _ho_debug_string(_comp, _obj, _str, _len)                              \
  do {                                                                         \
    __ho_debugger_run(&((struct ho_debug_desc){                                \
      .component = _comp,                                                      \
      .event = HO_COMP_EVT_MESSAGE,                                            \
      .timestamp = ho_get_tick(),                                              \
      .object = _obj,                                                          \
      .len = _len,                                                             \
      .data = (void **) _str,                                                  \
    }));                                                                       \
  } while (0)

#define ho_debug_string(_comp, _obj, _str)                                     \
  _ho_debug_string(_comp, _obj, _str, sizeof(_str) - 1)

#define ho_msg(_comp, _obj, _format, ...)                                      \
  do {                                                                         \
    char HO_LOCAL_VAR()[HO_DEBUG_MSG_LEN] = { 0 };                             \
    _ho_debug_string(                                                          \
      _comp, _obj, HO_LOCAL_VAR(), ({                                          \
        int l =                                                                \
          snprintf(HO_LOCAL_VAR(), HO_DEBUG_MSG_LEN, _format, ##__VA_ARGS__);  \
        (l > 0 ? l : 0);                                                       \
      }));                                                                     \
  } while (0)

#define ho_event(_comp, _event, _obj, ...)                                     \
  do {                                                                         \
    void **HO_LOCAL_VAR() = (void *[]){ NULL, ##__VA_ARGS__ };                 \
    __ho_debugger_run(&(struct ho_debug_desc){                                 \
      .component = _comp,                                                      \
      .event = _event,                                                         \
      .timestamp = ho_get_tick(),                                              \
      .object = _obj,                                                          \
      .len = (sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *)),             \
      .data = HO_LOCAL_VAR() + 1,                                              \
    });                                                                        \
  } while (0)

#define ho_log(_format, ...)                                                   \
  ho_msg(HO_COMP_EVT_MESSAGE, NULL, _format, ##__VA_ARGS__)

#define ho_assert(_cond)                                                       \
  ({                                                                           \
    bool res = _cond;                                                          \
    if (!res) {                                                                \
      __HO_ASSERT_STOP_THREAD();                                               \
      ho_event(HO_COMP_EVT_ASSERT,                                             \
               HO_COMP_EVT_ASSERT,                                             \
               #_cond,                                                         \
               __FILE__,                                                       \
               (void *) __LINE__);                                             \
      __HO_ASSERT_STOP_IRQ();                                                  \
    }                                                                          \
    (res);                                                                     \
  })

/* Weak --------------------------------------------------------------------- */
#define HO_DEBUGGER_DEFAULT_OUTPUT()                                           \
  void ho_debugger_default_output(                                             \
    const char *data HO_UNUSED, ho_u32 len HO_UNUSED)
extern HO_DEBUGGER_DEFAULT_OUTPUT();

/* Hard Fault --------------------------------------------------------------- */
extern void ho_hardfault_callback(void) HO_NORETURN;

/* FORMATTER ---------------------------------------------------------------- */
#define HO_DEBUG_FORMATTER_CALLBACK(_name)                                     \
  ho_u32 _name(struct ho_debug_desc *dbg HO_UNUSED, char **ptr HO_UNUSED)
#define HO_DEBUG_FORMATTER_PRINTF(_format, ...)                                \
  return ({                                                                    \
    int l = snprintf(*ptr, HO_DEBUG_MSG_LEN, _format, ##__VA_ARGS__);          \
    (l > 0 ? l : 0);                                                           \
  })
#define HO_DEBUG_FORMATTER_USE(_ptr, _len)                                     \
  do {                                                                         \
    *ptr = _ptr;                                                               \
    return _len;                                                               \
  } while (0)
#define HO_DEBUG_FORMATTER_ANONYMOUS()                                         \
  static HO_DEBUG_FORMATTER_CALLBACK(HO_LOCAL_VAR());                          \
  HO_LIST_ADD(ho_debugger_formatter, &HO_LOCAL_VAR());                         \
  static HO_DEBUG_FORMATTER_CALLBACK(HO_LOCAL_VAR())

/* Default ------------------------------------------------------------------ */
#if !defined(HO_DEBUG_MSG_LEN)
#define HO_DEBUG_MSG_LEN 128
#endif /* HO_DEBUG_MSG_LEN */

#if defined(HO_ASSERTION)
#define __HO_ASSERT_STOP_THREAD() HO_ENTER_CRITICAL()
#define __HO_ASSERT_STOP_IRQ() HO_MASK_IRQ()
#else
#define __HO_ASSERT_STOP_THREAD()
#define __HO_ASSERT_STOP_IRQ()
#endif /* HO_ASSERTION */

/**
 * @brief Debug Desc
 */
struct ho_debug_desc {
  ho_cstring component, event;
  const ho_u32 timestamp;
  const void *object;
  void *const *data;
  const ho_u32 len;
};

/**
 * @brief Debugger
 */
struct ho_debugger {
  void (*const process)(struct ho_debugger *, struct ho_debug_desc *);
  ho_payload payload;
};
/* List --------------------------------------------------------------------- */
HO_LIST_DECLARE(struct ho_debugger, ho_debugger);
/* Api ---------------------------------------------------------------------- */
static inline void __ho_debugger_run(struct ho_debug_desc *dbg) {
  HO_LIST_FOREACH(ho_debugger, debugger) {
    (*debugger)->process(*debugger, dbg);
  }
}

#endif /* ___HO_DEBUG_H___ */
