/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "ho_stm32_uart.h"
#include <ho/ho_list.h>
#include <ho/ho_init.h>

/* LIST --------------------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_stm32_uart_rx_handler, ho_stm32_uart_rx_handler);

/* INIT --------------------------------------------------------------------- */
HO_INIT_ANONYMOUS(ho_init_service) {
  HO_LIST_FOREACH(ho_stm32_uart_rx_handler, rx) {
    if ((*rx)->interface && (*rx)->interface->start) {
      (*rx)->interface->start(*rx);
    }
  }
}

/* CALLBACK ----------------------------------------------------------------- */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  HO_LIST_FOREACH(ho_stm32_uart_rx_handler, rx) {
    if (((*rx)->handler == huart) && ((*rx)->interface) &&
        ((*rx)->interface->receive)) {
      (*rx)->interface->receive(*rx);
    }
  }
}

/* WRITER ------------------------------------------------------------------- */
static bool put(void *handler, char byte) {
  return HAL_UART_Transmit(handler, (uint8_t *) &byte, 1, HO_MS(10)) == HAL_OK;
}
static ho_u32 write(void *handler, const char *data, ho_u32 len) {
  return HAL_UART_Transmit(handler, (uint8_t *) data, len, len * HO_MS(10)) ==
             HAL_OK ?
           len :
           0;
}

const struct ho_byte_writer_interface __ho_stm32_uart_byte_writer_interface = {
  .put = put, .write = write
};

/* RING BUFFER -------------------------------------------------------------- */
static void rb_start(struct ho_stm32_uart_rx_handler *self) {
  struct ho_stm32_uart_rx_data_rb *data = self->payload;
  HAL_UART_Receive_IT(self->handler, (uint8_t *) &(data->byte), 1);
}
static void rb_receive(struct ho_stm32_uart_rx_handler *self) {
  struct ho_stm32_uart_rx_data_rb *data = self->payload;
  if (!ho_rb_is_full(data->rb)) { ho_rb_push(data->rb, data->byte); }
  HAL_UART_Receive_IT(self->handler, (uint8_t *) &(data->byte), 1);
}
static void rb_error(struct ho_stm32_uart_rx_handler *self) {
  // TODO: when error
}
/* -------------------------------------------------------------------------- */
const struct ho_stm32_uart_rx_interface ho_stm32_uart_rx_interface_rb = {
  .receive = rb_receive,
  .start = rb_start,
  .error = rb_error,
};
