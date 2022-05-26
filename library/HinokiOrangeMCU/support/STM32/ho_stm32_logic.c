/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "ho_stm32_logic.h"

enum ho_logic __ho_stm32_gpio_read(void *payload) {
  struct __ho_stm32_gpio *gpio = payload;
  return (gpio->port->IDR & gpio->pin) > 0 ? HO_HIGH : HO_LOW;
}

void __ho_stm32_gpio_write(void *payload, enum ho_logic logic) {
  struct __ho_stm32_gpio *gpio = payload;
  volatile uint32_t *reg =
    ((logic == HO_HIGH) ? &gpio->port->BSRR : &gpio->port->BRR);
  *reg |= gpio->pin;
}
