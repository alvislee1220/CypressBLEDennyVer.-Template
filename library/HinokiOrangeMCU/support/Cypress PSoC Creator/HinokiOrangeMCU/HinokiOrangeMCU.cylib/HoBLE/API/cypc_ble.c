/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "`$INSTANCE_NAME`_cypc_ble.h"

/* LINKER ------------------------------------------------------------------- */
void *HO_LINKER_CYPC_BLE;

/* COMPONENT ---------------------------------------------------------------- */
ho_cstring HO_COMP_CYPC_BLE = "BLE";
ho_cstring HO_EVT_CYPC_BLE_COMMON = "Common";

/* LIST --------------------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_cypc_ble_listener, ho_cypc_ble_common_listeners);

/* COMMON ------------------------------------------------------------------- */
static void common_handler(unsigned long event, void *param) {
  HO_LIST_FOREACH(ho_cypc_ble_common_listeners, listener) {
    if ((*listener)->callback == NULL) { continue; }
    if ((*listener)->event != event) { continue; }
    ho_event(HO_COMP_CYPC_BLE, HO_EVT_CYPC_BLE_COMMON, *listener, param);
    (*listener)->callback(param);
  }
}
HO_INIT_ANONYMOUS(ho_init_service) { CyBle_Start(common_handler); }
HO_ROUTINE_ANONYMOUS() { CyBle_ProcessEvents(); }

/* ADVERTISING -------------------------------------------------------------- */
static void advertising(void *param) {
  CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
}
HO_LIST_ADD(ho_cypc_ble_common_listeners,
            (&((struct ho_cypc_ble_listener){
              .event = CYBLE_EVT_STACK_ON,
              .callback = advertising,
            })));
HO_LIST_ADD(ho_cypc_ble_common_listeners,
            (&((struct ho_cypc_ble_listener){
              .event = CYBLE_EVT_GAP_DEVICE_DISCONNECTED,
              .callback = advertising,
            })));

#if defined(CYBLE_IAS_SERVER)
/* IAS ---------------------------------------------------------------------- */
HO_WEAK HO_CYPC_BLE_IAS_CALLBACK() {}
static void ias_handler(unsigned long event, void *param) {
  ho_cypc_ble_ias_callback(*((CYBLE_IAS_CHAR_VALUE_T *) param)->value->val);
}
HO_INIT_ANONYMOUS(ho_init_service) {
  CyBle_IasRegisterAttrCallback(ias_handler);
}
#endif /** CYBLE_IAS_SERVER */
