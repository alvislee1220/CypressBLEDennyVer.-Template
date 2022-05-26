/* SPDX-License-Identifier: MIT */
#ifndef ___`$INSTANCE_NAME`_CYPC_UART_H___
#define ___`$INSTANCE_NAME`_CYPC_UART_H___
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_ringbuffer.h>
#include <ho/ho_byte_interface.h>

/* IDENTIFIER --------------------------------------------------------------- */
#define `$INSTANCE_NAME`_`$UART_INSTANCE`

/* DEFINED? ----------------------------------------------------------------- */
#if !defined(`$INSTANCE_NAME`_undefined)
#include "project.h"

#if !`$IS_SOFT_UART`
/*!< ring buffer for `$UART_INSTANCE` */
extern struct ho_rb `$UART_INSTANCE`_rb;

/*!< byte reader for `$UART_INSTANCE` */
extern struct ho_byte_reader `$UART_INSTANCE`_byte_reader;
#endif /** ONLY TX */

/*!< byte writer for `$UART_INSTANCE` */
extern struct ho_byte_writer `$UART_INSTANCE`_byte_writer;

/* SOFT --------------------------------------------------------------------- */
#if `$IS_SOFT_UART`
static inline void `$UART_INSTANCE`_UartPutChar(char byte) {
  `$UART_INSTANCE`_PutChar(byte);
}
static inline void  `$UART_INSTANCE`_UartPutString(const char *data) {
  `$UART_INSTANCE`_PutString(data);
}

#endif /** SOFT */

#endif /** `$INSTANCE_NAME` */

#endif /* ___`$INSTANCE_NAME`_CYPC_UART_H___ */
