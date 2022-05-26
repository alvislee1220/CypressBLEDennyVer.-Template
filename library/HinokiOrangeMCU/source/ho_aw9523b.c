/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_aw9523b.h>
#include <ho/ho_routine.h>
#include <ho/ho_countdown.h>

/* COMPONENT ---------------------------------------------------------------- */
ho_cstring HO_COMP_AW9523B = "AW9523B";

/* LOCAL -------------------------------------------------------------------- */
#define INITIALIZED (1U << 31)
#define WAIT_APPLY (1U << 30)

/* CHANGE ------------------------------------------------------------------- */
#define CHANGE_OUTPUT (1U << 24)
#define CHANGE_CTRL (1U << 25)
#define CHANGE_PIN_CONF (1U << 26)
#define CHANGE_DIM (1U << 27)
#define CHANGE_MASK (CHANGE_OUTPUT | CHANGE_CTRL | CHANGE_PIN_CONF | CHANGE_DIM)

/* MAP ---------------------------------------------------------------------- */
static const ho_u8 dim_map[2][8] = {
  /** P0 */
  [0] = { [0] = 4, [1] = 5, [2] = 6, [3] = 7, [4] = 8, [5] = 9, [6] = 10, [7] = 11 },
  /** P1 */
  [1] = { [0] = 0, [1] = 1, [2] = 2, [3] = 3, [4] = 12, [5] = 13, [6] = 14, [7] = 15 }
};

/* API ---------------------------------------------------------------------- */
bool ho_aw9523b_is_ready(struct ho_aw9523b *self) {
  return ho_i2c_ack(self->bus, self->addr, HO_MS(10));
}

bool ho_aw9523b_init(struct ho_aw9523b *self) {
  if (ho_has_any(self->flag, INITIALIZED)) { return true; }

  if (self->reset) {
    ho_logic_write(self->reset, HO_HIGH);
    ho_delay(HO_MS(50));
  }

  if (!ho_aw9523b_is_ready(self)) { return false; }
  /** check id */
  {
    struct ho_i2c_reg id = HO_I2C_REG_MAKE(1, 1, 0x10); /** read id from 0x10 */
    if (!ho_i2c_reg_read(self->bus, self->addr, &id, HO_MS(10))) {
      ho_event(HO_COMP_AW9523B, HO_EVT_FAILED, self, ho_i2c_reg_read, &id);
      return false;
    } else if (*ho_i2c_reg_data(&id) != 0x23) {
      ho_event(HO_COMP_AW9523B, HO_EVT_INVALID, self, ho_i2c_reg_data(&id));
      return false;
    }
  }
  /** init */
  {
    ho_aw9523b_wait_apply(self);
    ho_mask(self->flag, CHANGE_MASK); /** force write all */
    ho_aw9523b_config(self, self->flag & 0xFF);
    HO_LIST_STRUCT_FOREACH(&self->pins, pin) {
      ho_aw9523b_pin_config(*pin, (*pin)->flag & 0xFF);
    }
    if (!ho_aw9523b_apply(self)) {
      ho_event(HO_COMP_AW9523B, HO_EVT_FAILED, self, ho_aw9523b_apply);
      return false;
    }
  }
  /** ok */
  ho_mask(self->flag, INITIALIZED);
  ho_event(HO_COMP_AW9523B, HO_EVT_INIT, self);
  return true;
}

void ho_aw9523b_deinit(struct ho_aw9523b *self) {
  if (ho_none_of(self->flag, INITIALIZED)) { return; }

  if (self->reset) { ho_logic_write(self->reset, HO_LOW); }

  ho_unmask(self->flag, INITIALIZED);
  ho_event(HO_COMP_AW9523B, HO_EVT_DEINIT, self);
}
/* -------------------------------------------------------------------------- */
bool ho_aw9523b_config(struct ho_aw9523b *self, ho_u8 config) {
  *ho_i2c_reg_data(&self->ctrl) = config;
  if (ho_has_any(self->flag, WAIT_APPLY)) {
    ho_mask(self->flag, CHANGE_CTRL);
    return true;
  }
  if (ho_none_of(self->flag, INITIALIZED)) { return false; }
  return ho_i2c_reg_write(self->bus, self->addr, &self->ctrl, HO_MS(100));
}

bool ho_aw9523b_config_pin(struct ho_aw9523b *self,
                           ho_u8 port,
                           ho_u8 pin,
                           ho_u8 config) {
  switch (config) {
  case HO_AW9523B_PIN_INPUT:
    ho_i2c_reg_data(&self->dir)[port] |= (1 << pin);
    ho_i2c_reg_data(&self->pin_mode)[port] |= (1 << pin);
    break;
  case HO_AW9523B_PIN_OUTPUT:
    ho_i2c_reg_data(&self->dir)[port] &= ~(1 << pin);
    ho_i2c_reg_data(&self->pin_mode)[port] |= (1 << pin);
    break;
  case HO_AW9523B_PIN_LED:
    ho_i2c_reg_data(&self->pin_mode)[port] &= ~(1 << pin);
    break;
  default:
    ho_event(HO_COMP_AW9523B, HO_EVT_INVALID, self, &config);
    return false;
  }
  if (ho_has_any(self->flag, WAIT_APPLY)) {
    ho_mask(self->flag, CHANGE_PIN_CONF);
    return true;
  }
  if (ho_none_of(self->flag, INITIALIZED)) { return false; }
  return ho_i2c_reg_write(self->bus, self->addr, &self->dir, HO_MS(100)) &&
         ho_i2c_reg_write(self->bus, self->addr, &self->pin_mode, HO_MS(100));
}
/* -------------------------------------------------------------------------- */
void ho_aw9523b_wait_apply(struct ho_aw9523b *self) {
  ho_mask(self->flag, WAIT_APPLY);
}

bool ho_aw9523b_apply(struct ho_aw9523b *self) {
  if (ho_none_of(self->flag, WAIT_APPLY)) { return true; }
  ho_unmask(self->flag, WAIT_APPLY);
  bool res = true;
  if (ho_has_any(self->flag, CHANGE_CTRL)) {
    res &= ho_i2c_reg_write(self->bus, self->addr, &self->ctrl, HO_MS(100));
  }
  if (ho_has_any(self->flag, CHANGE_PIN_CONF)) {
    res &= ho_i2c_reg_write(self->bus, self->addr, &self->dir, HO_MS(100)) &&
           ho_i2c_reg_write(self->bus, self->addr, &self->pin_mode, HO_MS(100));
  }
  if (ho_has_any(self->flag, CHANGE_OUTPUT)) {
    res &= ho_i2c_reg_write(self->bus, self->addr, &self->output, HO_MS(100));
  }
  if (ho_has_any(self->flag, CHANGE_DIM)) {
    res &= ho_i2c_reg_write(self->bus, self->addr, &self->dim, HO_MS(200));
  }
  ho_unmask(self->flag, CHANGE_MASK);
  return res;
}
/* -------------------------------------------------------------------------- */
enum ho_logic ho_aw9523b_read(struct ho_aw9523b *self, ho_u8 port, ho_u8 pin) {
  return (ho_i2c_reg_data(&self->input)[port] & (1 << pin)) > 0 ? HO_HIGH :
                                                                  HO_LOW;
}

bool ho_aw9523b_write(struct ho_aw9523b *self,
                      ho_u8 port,
                      ho_u8 pin,
                      enum ho_logic logic) {
  ho_i2c_reg_data(&self->output)[port] &= ~(1 << pin);
  ho_i2c_reg_data(&self->output)[port] |= (logic << pin);
  if (ho_has_any(self->flag, WAIT_APPLY)) {
    ho_mask(self->flag, CHANGE_OUTPUT);
    return true;
  }
  if (ho_none_of(self->flag, INITIALIZED)) { return false; }
  return ho_i2c_reg_write(self->bus, self->addr, &self->output, HO_MS(100));
}

ho_u8 ho_aw9523b_read_dim(struct ho_aw9523b *self, ho_u8 port, ho_u8 pin) {
  return ho_i2c_reg_data(&self->dim)[dim_map[port][pin]];
}

bool ho_aw9523b_write_dim(struct ho_aw9523b *self,
                          ho_u8 port,
                          ho_u8 pin,
                          ho_u8 dim) {
  ho_i2c_reg_data(&self->dim)[dim_map[port][pin]] = dim;
  if (ho_has_any(self->flag, WAIT_APPLY)) {
    ho_mask(self->flag, CHANGE_DIM);
    return true;
  }
  if (ho_none_of(self->flag, INITIALIZED)) { return false; }
  return ho_i2c_reg_write(self->bus, self->addr, &self->dim, HO_MS(100));
}
/* -------------------------------------------------------------------------- */
HO_ROUTINE_CALLBACK(__ho_aw9523b_routine_callback) {
  struct ho_aw9523b *dev = routine->payload;
  /** is initialized */
  if (ho_none_of(dev->flag, INITIALIZED)) { return; }
  do {
    if ((ho_i2c_reg_data(&dev->dir)[0] & ho_i2c_reg_data(&dev->pin_mode)[0]) >
        0) {
      break;
    }
    if ((ho_i2c_reg_data(&dev->dir)[1] & ho_i2c_reg_data(&dev->pin_mode)[1]) >
        0) {
      break;
    }
    return;
  } while (0);

  /** trigger fetch input */
  if (dev->isr) {
    if (ho_logic_read(dev->isr) != HO_LOW) { return; }
  } else if (!ho_is_ticks_up(dev->_ts, HO_MS(10))) {
    return;
  }
  dev->_ts = ho_get_tick();

  /** fetch input */
  if (!ho_i2c_reg_read(dev->bus, dev->addr, &dev->input, HO_MS(100))) {
    ho_event(HO_COMP_AW9523B, HO_EVT_FAILED, dev, ho_i2c_reg_read);
  }
}
