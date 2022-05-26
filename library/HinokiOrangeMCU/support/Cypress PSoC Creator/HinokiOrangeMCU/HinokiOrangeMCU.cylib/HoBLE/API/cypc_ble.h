/* SPDX-License-Identifier: MIT */
#ifndef ___CYPC_BLE_H___
#define ___CYPC_BLE_H___

/* INC ---------------------------------------------------------------------- */
#include "project.h"

/* COMP --------------------------------------------------------------------- */
extern ho_cstring HO_COMP_CYPC_BLE;
extern ho_cstring HO_EVT_CYPC_BLE_COMMON;

/* DECLARE ------------------------------------------------------------------ */
struct ho_cypc_ble_listener;
/* API ---------------------------------------------------------------------- */
#define HO_CYPC_BLE_CALLBACK(_name) void _name(void *param HO_UNUSED)
#define HO_CYPC_BLE_ANONYMOUS(_listeners, _event)                              \
  static HO_CYPC_BLE_CALLBACK(HO_LOCAL_VAR());                                 \
  HO_LIST_ADD(_listeners,                                                      \
              (&((struct ho_cypc_ble_listener){                                \
                .event = _event, .callback = HO_LOCAL_VAR() })));              \
  static HO_CYPC_BLE_CALLBACK(HO_LOCAL_VAR())
/* LIST --------------------------------------------------------------------- */
extern void *ho_cypc_ble_common_listeners; /*!< fake for auto-complete */

#if defined(CYBLE_IAS_SERVER)
#define HO_CYPC_BLE_IAS_CALLBACK()                                             \
  void ho_cypc_ble_ias_callback(ho_u8 value HO_UNUSED)
extern HO_CYPC_BLE_IAS_CALLBACK();
#endif /** CYBLE_IAS_SERVER */

/**
 * @brief Listener
 */
struct ho_cypc_ble_listener {
  unsigned long event;
  void (*const callback)(void *);
};

#endif /* ___CYPC_BLE_H___ */
