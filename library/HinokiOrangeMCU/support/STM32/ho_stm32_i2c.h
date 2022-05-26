/* SPDX-License-Identifier: MIT */
#ifndef ___HO_STM32_I2C_H___
#define ___HO_STM32_I2C_H___
/* INC ---------------------------------------------------------------------- */
#include HO_STM32_HEADER
#include <ho/peripheral/ho_i2c.h>

/* DECLARE ------------------------------------------------------------------ */
#define HO_STM32_I2C_MAKE(_i2c)                                                \
  { .payload = _i2c, .interface = &__ho_stm32_i2c_interface };

/* INTERFACE ---------------------------------------------------------------- */
extern struct ho_i2c_interface __ho_stm32_i2c_interface;

#endif /* ___HO_STM32_I2C_H___ */
