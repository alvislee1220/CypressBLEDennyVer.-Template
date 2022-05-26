/* SPDX-License-Identifier: MIT */
#ifndef ___HO_UART_H___
#define ___HO_UART_H___

/* DECLARE ------------------------------------------------------------------ */
#define HO_UART_RX_CALLBACK_NAME(_name) __ho_uart_rx_##_name##_callback
#define HO_UART_RX_CALLBACK(_name)                                             \
  void HO_UART_RX_CALLBACK_NAME(_name)(char byte HO_UNUSED)

#endif /* ___HO_UART_H___ */
