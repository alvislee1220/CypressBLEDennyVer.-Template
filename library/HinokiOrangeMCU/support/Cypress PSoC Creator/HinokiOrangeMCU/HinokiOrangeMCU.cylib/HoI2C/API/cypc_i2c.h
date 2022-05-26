/* SPDX-License-Identifier: MIT */
#ifndef ___`$INSTANCE_NAME`_CYPC_I2C_H___
#define ___`$INSTANCE_NAME`_CYPC_I2C_H___

/* BUS ---------------------------------------------------------------------- */
#include <ho/peripheral/ho_i2c.h>
extern struct ho_i2c_bus `$I2C_INSTANCE`_bus;

/* HELPER ------------------------------------------------------------------- */
// clang-format off
#define HO_ENSURE_USE_`$I2C_INSTANCE`_1 \
    static struct ho_i2c_bus *__used_`$I2C_INSTANCE`_bus HO_USED = &`$I2C_INSTANCE`_bus
// clang-format on

#endif /* ___`$INSTANCE_NAME`_CYPC_I2C_H___ */
