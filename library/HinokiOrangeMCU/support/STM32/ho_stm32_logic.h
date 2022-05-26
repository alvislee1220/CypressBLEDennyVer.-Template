/* SPDX-License-Identifier: MIT */
#ifndef ___HO_STM32_LOGIC_H___
#define ___HO_STM32_LOGIC_H___

/* INC ---------------------------------------------------------------------- */
#include HO_STM32_HEADER
#include <ho/ho_logic.h>

/* DECLARE ------------------------------------------------------------------ */
#define HO_STM32_LOGIC_READER_MAKE(_gpio)                                      \
  { .read = __ho_stm32_gpio_read, .payload = &(__HO_STM32_GPIO_MAKE(_gpio)) }
#define HO_STM32_LOGIC_WRITER_MAKE(_gpio)                                      \
  { .write = __ho_stm32_gpio_write, .payload = &(__HO_STM32_GPIO_MAKE(_gpio)) }

/**
 * @brief GPIO Definition
 */
struct __ho_stm32_gpio {
  volatile GPIO_TypeDef *const port;
  uint32_t pin;
};
/* -------------------------------------------------------------------------- */
#define __HO_STM32_GPIO_MAKE(_gpio)                                            \
  (struct __ho_stm32_gpio) { .port = _gpio##_GPIO_Port, .pin = _gpio##_Pin }
/* -------------------------------------------------------------------------- */
enum ho_logic __ho_stm32_gpio_read(void *);
void __ho_stm32_gpio_write(void *, enum ho_logic);

#endif /* ___HO_STM32_LOGIC_H___ */
