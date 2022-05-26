/* SPDX-License-Identifier: MIT */

#ifndef ___HO_INIT_H___
#define ___HO_INIT_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_list.h"
#include "ho_debug.h"

/* Debug -------------------------------------------------------------------- */
extern ho_cstring HO_COMP_INIT;
extern ho_cstring HO_COMP_INIT_STAGE;

/* ************************* Initialize After Reset ************************* */

/*!< [list] callback when init after reset */
typedef void (*ho_init_after_reset_callback)(void);
/* List --------------------------------------------------------------------- */
HO_LIST_DECLARE(ho_init_after_reset_callback, ho_init_after_reset_callback);
/* Api ---------------------------------------------------------------------- */
static inline void ho_process_init_after_reset(void);

/* ********************************** Init ********************************** */

struct ho_init; /*!< init */
struct ho_init_stage; /*!< init stage */
/* List --------------------------------------------------------------------- */
extern struct ho_init_stage ho_init_clock; /*!< [list] clock init stage */
extern struct ho_init_stage ho_init_peripheral; /*!< [list] clock init
                                                   peripheral */
extern struct ho_init_stage ho_init_library; /*!< [list] library init*/
extern struct ho_init_stage ho_init_service; /*!< [list] service init */
extern struct ho_init_stage ho_init_app; /*!< [list] init application */
/* Api ---------------------------------------------------------------------- */
static inline void ho_process_init(void);
/* -------------------------------------------------------------------------- */
#define HO_INIT_CALLBACK(_name) void _name(struct ho_init *init HO_UNUSED)
#define HO_INIT_ADD(_stage, _callback, ...)                                    \
  HO_LIST_ADD(                                                                 \
    _stage, (&((struct ho_init){ .callback = _callback, ##__VA_ARGS__ })))
#define HO_INIT_ANONYMOUS(_stage, ...)                                         \
  static HO_INIT_CALLBACK(HO_LOCAL_VAR()) HO_USED;                             \
  HO_INIT_ADD(_stage, HO_LOCAL_VAR(), ##__VA_ARGS__);                          \
  static HO_USED HO_INIT_CALLBACK(HO_LOCAL_VAR())

/* ******************************** Implement ******************************* */

/* Init --------------------------------------------------------------------- */
struct ho_init {
  void (*const callback)(struct ho_init *);
  ho_payload payload;
};
/* Init Stage --------------------------------------------------------------- */
HO_LIST_STRUCT(struct ho_init, ho_init_stage);
HO_LIST_DECLARE(struct ho_init_stage, __ho_init_list);
/* Api --------------------------------------------------------------------- */
static inline void ho_process_init_after_reset(void) {
  HO_LIST_FOREACH(ho_init_after_reset_callback, init) { (**init)(); }
}

static inline void ho_process_init(void) {
  HO_LIST_FOREACH(__ho_init_list, list) {
    ho_event(HO_COMP_INIT_STAGE, HO_EVT_RUN, *list);
    HO_LIST_STRUCT_FOREACH((*list), init) {
      ho_event(HO_COMP_INIT, HO_EVT_RUN, *init, *list);
      (*init)->callback(*init);
    }
  }
}

#endif /* ___HO_INIT_H___ */
