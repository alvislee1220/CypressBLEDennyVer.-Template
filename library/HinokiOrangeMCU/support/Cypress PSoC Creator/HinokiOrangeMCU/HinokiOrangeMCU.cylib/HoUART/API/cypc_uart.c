/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "`$INSTANCE_NAME`_cypc_uart.h"

#if !defined(`$INSTANCE_NAME`_undefined)
#include "project.h"

#if `$IS_SOFT_UART`
#define ONLY_TX
#define SOFT_ALIAS
#endif /** SOFT */

#if !defined(ONLY_TX)
/* RINGBUFFER --------------------------------------------------------------- */
struct ho_rb `$UART_INSTANCE`_rb = HO_RB_MAKE(`$RING_BUFFER_SIZE`);

/* READER ------------------------------------------------------------------- */
struct ho_byte_reader `$UART_INSTANCE`_byte_reader =
  HO_RB_BYTE_READER_MAKE(&`$UART_INSTANCE`_rb);
#endif /** SOFT */

/* WRITER ------------------------------------------------------------------- */
static HO_BYTE_WRITER_PUT_CALLBACK(put) {
  `$UART_INSTANCE`_UartPutChar(byte);
  return true;
}

static HO_BYTE_WRITER_WRITE_CALLBACK(write) {
  `$UART_INSTANCE`_UartPutString(data);
  return len;
}

struct ho_byte_writer `$UART_INSTANCE`_byte_writer = {
  .interface =
    (&((struct ho_byte_writer_interface){ .put = put, .write = write }))
};

#if !defined(ONLY_TX)
/* ISR ---------------------------------------------------------------------- */
static CY_ISR(rx_isr_callback) {
  if (`$UART_INSTANCE`_GetRxInterruptSourceMasked() & `$UART_INSTANCE`_INTR_RX_NOT_EMPTY) {
    if (!ho_rb_is_full(&`$UART_INSTANCE`_rb)) {
      ho_rb_push(&`$UART_INSTANCE`_rb, `$UART_INSTANCE`_UartGetChar());
    }
    `$UART_INSTANCE`_ClearRxInterruptSource(`$UART_INSTANCE`_INTR_RX_NOT_EMPTY);
  }
}
#endif /** ONLY TX */
/* INIT --------------------------------------------------------------------- */
HO_INIT_ANONYMOUS(ho_init_peripheral) {
#if !defined(ONLY_TX)
  `$UART_INSTANCE`_SetCustomInterruptHandler(rx_isr_callback);
#endif /** ONLY TX */
  `$UART_INSTANCE`_Start();
  /** start */
}

#else /** un-assigned */
#warning `$INSTANCE_NAME` not be assigned UART INSTANCE
#endif /** `$INSTANCE_NAME` */
