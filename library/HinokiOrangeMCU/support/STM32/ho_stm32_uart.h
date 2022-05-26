/* SPDX-License-Identifier: MIT */
#ifndef ___HO_STM32_UART_H___
#define ___HO_STM32_UART_H___

/* INC ---------------------------------------------------------------------- */
#include HO_STM32_HEADER
#include <ho/ho_byte_interface.h>
#include <ho/ho_ringbuffer.h>

/* RX HANDLER --------------------------------------------------------------- */
struct ho_stm32_uart_rx_handler;
struct ho_stm32_uart_rx_interface;

/* DECLARE ------------------------------------------------------------------ */
#define HO_STM32_UART_DECLARE(_uart)                                           \
  extern struct ho_byte_reader _uart##_byte_reader;                            \
  extern struct ho_byte_writer _uart##_byte_writer;
/* INSTANCE ----------------------------------------------------------------- */
#define HO_STM32_UART_INSTANCE(_uart, _size)                                   \
  static struct ho_rb _uart##_rb = HO_RB_MAKE(_size);                          \
  static struct ho_stm32_uart_rx_handler _uart##_rx = {                        \
    .handler = &_uart,                                                         \
    .interface = &ho_stm32_uart_rx_interface_rb,                               \
    .payload = (&((struct ho_stm32_uart_rx_data_rb){ .rb = &_uart##_rb }))     \
  };                                                                           \
  struct ho_byte_reader _uart##_byte_reader =                                  \
    HO_RB_BYTE_READER_MAKE(&_uart##_rb);                                       \
  struct ho_byte_writer _uart##_byte_writer = {                                \
    .interface = &__ho_stm32_uart_byte_writer_interface, .payload = &_uart     \
  };                                                                           \
  HO_LIST_ADD(ho_stm32_uart_rx_handler, &_uart##_rx);

/* INTERFACE ---------------------------------------------------------------- */
extern const struct ho_byte_writer_interface
  __ho_stm32_uart_byte_writer_interface;

/**
 * @brief handler
 */
struct ho_stm32_uart_rx_handler {
  UART_HandleTypeDef *const handler;
  const struct ho_stm32_uart_rx_interface *const interface;
  ho_payload payload;
};

/**
 * @brief interface
 */
struct ho_stm32_uart_rx_interface {
  void (*const start)(struct ho_stm32_uart_rx_handler *);
  void (*const receive)(struct ho_stm32_uart_rx_handler *);
  void (*const error)(struct ho_stm32_uart_rx_handler *);
};

/* RING BUFFER -------------------------------------------------------------- */
struct ho_stm32_uart_rx_data_rb {
  struct ho_rb *const rb;
  char byte;
};
/* -------------------------------------------------------------------------- */
extern const struct ho_stm32_uart_rx_interface ho_stm32_uart_rx_interface_rb;

#endif /* ___HO_STM32_UART_H___ */
