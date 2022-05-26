/* SPDX-License-Identifier: MIT */
#ifndef ___HO_AW9523B_H___
#define ___HO_AW9523B_H___

/* INC ---------------------------------------------------------------------- */
#include <ho/peripheral/ho_i2c.h>
#include <ho/ho_logic.h>

/* COMPONENT ---------------------------------------------------------------- */
extern ho_cstring HO_COMP_AW9523B;
/* DEVICE ------------------------------------------------------------------- */
struct ho_aw9523b;
struct ho_aw9523b_pin;
/* --------------------------------- Current -------------------------------- */
#define HO_AW9523B_37mA (0 << 0)
#define HO_AW9523B_27mA (1 << 0)
#define HO_AW9523B_18mA (2 << 0)
#define HO_AW9523B_9mA (3 << 0)
/* --------------------------------- P0 Mode -------------------------------- */
#define HO_AW9523B_P0_OD (0 << 4)
#define HO_AW9523B_P0_PP (1 << 4)
/* -------------------------------- Pin Mode -------------------------------- */
#define HO_AW9523B_PIN_INPUT (0 << 0)
#define HO_AW9523B_PIN_OUTPUT (1 << 0)
#define HO_AW9523B_PIN_LED (2 << 0)
/* API ---------------------------------------------------------------------- */
bool ho_aw9523b_is_ready(struct ho_aw9523b *);
bool ho_aw9523b_init(struct ho_aw9523b *);
void ho_aw9523b_deinit(struct ho_aw9523b *);
/* -------------------------------------------------------------------------- */
bool ho_aw9523b_config(struct ho_aw9523b *, ho_u8);
bool ho_aw9523b_config_pin(struct ho_aw9523b *, ho_u8, ho_u8, ho_u8);
/* -------------------------------------------------------------------------- */
void ho_aw9523b_wait_apply(struct ho_aw9523b *);
bool ho_aw9523b_apply(struct ho_aw9523b *);
/* -------------------------------------------------------------------------- */
enum ho_logic ho_aw9523b_read(struct ho_aw9523b *, ho_u8, ho_u8);
bool ho_aw9523b_write(struct ho_aw9523b *, ho_u8, ho_u8, enum ho_logic);
ho_u8 ho_aw9523b_read_dim(struct ho_aw9523b *, ho_u8, ho_u8);
bool ho_aw9523b_write_dim(struct ho_aw9523b *, ho_u8, ho_u8, ho_u8);
/* -------------------------------------------------------------------------- */
#define HO_AW9523B_INSTANCE(_var, _bus, _a1, _a0, ...)                         \
  HO_LIST_INSTANCE(struct ho_aw9523b_pin, _var);                               \
  struct ho_aw9523b _var =                                                     \
    __HO_AW9523B_MAKE(_var, _bus, _a1, _a0, ##__VA_ARGS__);                    \
  __HO_ROUTINE_ADD_AW9523B(&_var);
/* PIN ---------------------------------------------------------------------- */
static inline bool ho_aw9523b_pin_config(struct ho_aw9523b_pin *, ho_u8);
static inline enum ho_logic ho_aw9523b_pin_read(struct ho_aw9523b_pin *);
static inline bool ho_aw9523b_pin_write(struct ho_aw9523b_pin *, enum ho_logic);
static inline ho_u8 ho_aw9523b_pin_read_dim(struct ho_aw9523b_pin *);
static inline bool ho_aw9523b_pin_write_dim(struct ho_aw9523b_pin *, ho_u8);
/* -------------------------------------------------------------------------- */
#define HO_AW9523B_PIN_INSTANCE(_var, _dev, _port, _pin, ...)                  \
  struct ho_aw9523b_pin _var =                                                 \
    (struct ho_aw9523b_pin){ .dev = &_dev,                                     \
                             .flag = HO_AW9523B_PIN_INPUT,                     \
                             .port = _port,                                    \
                             .pin = _pin,                                      \
                             ##__VA_ARGS__ };                                  \
  HO_LIST_ADD(_dev, &_var);

/**
 * @brief AW9523B Device
 */
struct ho_aw9523b {
  short addr; /*!< device addr = 0x5B | A1 | A0 */
  struct ho_i2c_bus *bus;
  ho_flag flag;
  struct ho_logic_writer *reset;
  struct ho_logic_reader *isr;
  struct ho_i2c_reg
    /** 0x00,2 */ input,
    /** 0x02,2 */ output,
    /** 0x04,2 */ dir,
    /** 0x11,1 */ ctrl,
    /** 0x12,2 */ pin_mode,
    /** 0x20,16 */ dim;
  HO_LIST_STRUCT(struct ho_aw9523b_pin) pins;
  ho_u32 _ts;
};
/* -------------------------------------------------------------------------- */
extern HO_ROUTINE_CALLBACK(__ho_aw9523b_routine_callback);
/* -------------------------------------------------------------------------- */
#define __HO_AW9523B_MAKE(_var, _bus, _a1, _a0, ...)                           \
  (struct ho_aw9523b) {                                                        \
    .addr = 0x5B | ((_a1) << 1) | ((_a0) << 0), .bus = _bus,                   \
    .input = HO_I2C_REG_MAKE(1, 2, 0x00),                                      \
    .output = HO_I2C_REG_MAKE(1, 2, 0x02), .dir = HO_I2C_REG_MAKE(1, 2, 0x04), \
    .ctrl = HO_I2C_REG_MAKE(1, 1, 0x11),                                       \
    .pin_mode = HO_I2C_REG_MAKE(1, 2, 0x12, 0xFF, 0xFF),                       \
    .dim = HO_I2C_REG_MAKE(1, 16, 0x20), .pins = HO_LIST_STRUCT_MAKE(_var),    \
    ##__VA_ARGS__                                                              \
  }
/* -------------------------------------------------------------------------- */
#define __HO_ROUTINE_ADD_AW9523B(_var)                                         \
  HO_ROUTINE_ADD(__ho_aw9523b_routine_callback,                                \
                 .name = #_var,                                                \
                 .flag = HO_ENABLE,                                            \
                 .payload = _var)

/**
 * @brief pin
 */
struct ho_aw9523b_pin {
  ho_u8 port, pin;
  struct ho_aw9523b *dev;
  ho_flag flag;
};
/* API ---------------------------------------------------------------------- */
static inline bool ho_aw9523b_pin_config(struct ho_aw9523b_pin *pin,
                                         ho_u8 config) {
  return ho_aw9523b_config_pin(pin->dev, pin->port, pin->pin, config);
}
static inline enum ho_logic ho_aw9523b_pin_read(struct ho_aw9523b_pin *pin) {
  return ho_aw9523b_read(pin->dev, pin->port, pin->pin);
}
static inline bool ho_aw9523b_pin_write(struct ho_aw9523b_pin *pin,
                                        enum ho_logic logic) {
  return ho_aw9523b_write(pin->dev, pin->port, pin->pin, logic);
}
static inline ho_u8 ho_aw9523b_pin_read_dim(struct ho_aw9523b_pin *pin) {
  return ho_aw9523b_read_dim(pin->dev, pin->port, pin->pin);
}
static inline bool ho_aw9523b_pin_write_dim(struct ho_aw9523b_pin *pin,
                                            ho_u8 dim) {
  return ho_aw9523b_write_dim(pin->dev, pin->port, pin->pin, dim);
}

#endif /* ___HO_AW9523B_H___ */
