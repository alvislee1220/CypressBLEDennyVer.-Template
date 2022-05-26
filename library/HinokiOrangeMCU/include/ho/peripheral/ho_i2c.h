/* SPDX-License-Identifier: MIT */
#ifndef ___HO_I2C_H___
#define ___HO_I2C_H___
/* INC ---------------------------------------------------------------------- */
#include <stdbool.h>
#include <ho/ho_typedef.h>

/* DEBUG -------------------------------------------------------------------- */
extern ho_cstring HO_COMP_I2C;
/* DECLARE ------------------------------------------------------------------ */
struct ho_i2c_interface;
struct ho_i2c_bus;
struct ho_i2c_reg;
/* API ---------------------------------------------------------------------- */
static inline bool ho_i2c_ack(struct ho_i2c_bus *, ho_u16, ho_u32);
static inline bool ho_i2c_transfer(
  struct ho_i2c_bus *, ho_u16, ho_u8 *, ho_u32, ho_u32, ho_u8 *, ho_u32);
static inline bool ho_i2c_transfer_async(
  struct ho_i2c_bus *, ho_u16, ho_u8 *, ho_u32, ho_u32, ho_u8 *);
static inline bool ho_i2c_is_ready(struct ho_i2c_bus *);
static inline ho_u32 ho_i2c_get_state(struct ho_i2c_bus *);
/* -------------------------------------------------------------------------- */
void ho_i2c_scan(struct ho_i2c_bus *, ho_u32, void (*)(ho_u16));
/* REG ---------------------------------------------------------------------- */
#define HO_I2C_REG_MAKE(_reg_len, _data_len, ...)                              \
  {                                                                            \
    .data = (ho_u8[(_reg_len) + (_data_len)]){ __VA_ARGS__ },                  \
    .data_len = (_data_len), .reg_len = (_reg_len)                             \
  }
/* -------------------------------------------------------------------------- */
static inline ho_u8 *ho_i2c_reg_data(struct ho_i2c_reg *);
static inline bool
ho_i2c_reg_write(struct ho_i2c_bus *, ho_u16, struct ho_i2c_reg *, ho_u32);
static inline bool
ho_i2c_reg_read(struct ho_i2c_bus *, ho_u16, struct ho_i2c_reg *, ho_u32);
static inline bool
ho_i2c_reg_write_async(struct ho_i2c_bus *, ho_u16, struct ho_i2c_reg *);
static inline bool
ho_i2c_reg_read_async(struct ho_i2c_bus *, ho_u16, struct ho_i2c_reg *);

/* CLI ---------------------------------------------------------------------- */
#include <ho/ho_cli.h>
extern struct ho_cli_cmds ho_cli_cmds_i2c;
HO_CLI_CMD_CALLBACK(ho_cli_cmd_i2c_callback);
#define HO_I2C_CLI_CMD_INSTANCE(_name, _cmd, _bus, ...)                        \
  HO_CLI_CMD_INSTANCE(_name,                                                   \
                      _cmd,                                                    \
                      ho_cli_cmd_i2c_callback,                                 \
                      .payload = _bus,                                         \
                      .children = &ho_cli_cmds_i2c,                            \
                      ##__VA_ARGS__);                                          \
  HO_LIST_ADD(ho_cli_cmds_default, &_name);

/**
 * @brief I2C Interface
 */
struct ho_i2c_interface {
  bool (*const ack)(void *, ho_u16, ho_u32);
  bool (*const transfer)(
    void *, ho_u16, ho_u8 *, ho_u32, ho_u32, ho_u8 *, ho_u32);
  bool (*const transfer_async)(void *, ho_u16, ho_u8 *, ho_u32, ho_u32, ho_u8 *);
  bool (*const is_ready)(void *);
  ho_u32 (*const get_state)(void *);
};

/**
 * @brief Bus definition
 */
struct ho_i2c_bus {
  struct ho_i2c_interface *const interface;
  ho_payload payload;
};
/* -------------------------------------------------------------------------- */
static inline bool
ho_i2c_ack(struct ho_i2c_bus *self, ho_u16 addr, ho_u32 timeout) {
  return self->interface->ack(self->payload, addr, timeout);
}
static inline bool ho_i2c_transfer(struct ho_i2c_bus *self,
                                   ho_u16 addr,
                                   ho_u8 *wdata,
                                   ho_u32 wlen,
                                   ho_u32 rlen,
                                   ho_u8 *rbuf,
                                   ho_u32 timeout) {
  return self->interface->transfer(
    self->payload, addr, wdata, wlen, rlen, rbuf, timeout);
}
static inline bool ho_i2c_transfer_async(struct ho_i2c_bus *self,
                                         ho_u16 addr,
                                         ho_u8 *wdata,
                                         ho_u32 wlen,
                                         ho_u32 rlen,
                                         ho_u8 *rbuf) {
  return self->interface->transfer_async(
    self->payload, addr, wdata, wlen, rlen, rbuf);
}
static inline ho_u32 ho_i2c_get_state(struct ho_i2c_bus *self) {
  return self->interface->get_state(self->payload);
}
static inline bool ho_i2c_is_ready(struct ho_i2c_bus *self) {
  return self->interface->is_ready(self->payload);
}

/**
 * @brief I2C Reg
 */
struct ho_i2c_reg {
  ho_u8 *data;
  const ho_u32 reg_len;
  const ho_u32 data_len;
};
/* IMPL --------------------------------------------------------------------- */
static inline ho_u8 *ho_i2c_reg_data(struct ho_i2c_reg *reg) {
  return reg->data + reg->reg_len;
}

static inline bool ho_i2c_reg_write(struct ho_i2c_bus *self,
                                    ho_u16 addr,
                                    struct ho_i2c_reg *reg,
                                    ho_u32 timeout) {
  return ho_i2c_transfer(
    self, addr, reg->data, reg->data_len + reg->reg_len, 0, NULL, timeout);
}
static inline bool ho_i2c_reg_write_async(struct ho_i2c_bus *self,
                                          ho_u16 addr,
                                          struct ho_i2c_reg *reg) {
  return ho_i2c_transfer_async(
    self, addr, reg->data, reg->data_len + reg->reg_len, 0, NULL);
}
static inline bool ho_i2c_reg_read(struct ho_i2c_bus *self,
                                   ho_u16 addr,
                                   struct ho_i2c_reg *reg,
                                   ho_u32 timeout) {
  return ho_i2c_transfer(self,
                         addr,
                         reg->data,
                         reg->reg_len,
                         reg->data_len,
                         ho_i2c_reg_data(reg),
                         timeout);
}
static inline bool ho_i2c_reg_read_async(struct ho_i2c_bus *self,
                                         ho_u16 addr,
                                         struct ho_i2c_reg *reg) {
  return ho_i2c_transfer_async(
    self, addr, reg->data, reg->reg_len, reg->data_len, ho_i2c_reg_data(reg));
}

#endif /* ___HO_I2C_H___ */
