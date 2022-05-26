/* SPDX-License-Identifier: MIT */
#ifndef ___HO_STM32_DEBUG_H___
#define ___HO_STM32_DEBUG_H___

#define HO_STM32_DEBUG_OUTPUT(_huart)                                          \
  HO_DEBUGGER_DEFAULT_OUTPUT() {                                               \
    HAL_UART_Transmit(_huart, (uint8_t *) data, len, len * 10);                \
  }

#endif /* ___HO_STM32_DEBUG_H___ */
