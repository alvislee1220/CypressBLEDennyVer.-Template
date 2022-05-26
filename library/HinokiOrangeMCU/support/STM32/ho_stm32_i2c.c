/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "ho_stm32_i2c.h"
#include <ho/ho_countdown.h>

static bool ack(void *self HO_UNUSED, ho_u16 addr, ho_u32 timeout) {
  return HAL_I2C_IsDeviceReady(self, addr << 1, 3, HO_MS(10)) == HAL_OK;
}

static bool transfer(void *self HO_UNUSED,
                     ho_u16 addr,
                     ho_u8 *wd,
                     ho_u32 wl,
                     ho_u32 rl,
                     ho_u8 *rd,
                     ho_u32 timeout) {
  struct ho_cd tmo;
  ho_cd_recount(&tmo);

  do {
    if /** timeout */ (ho_cd_is_done(&tmo, timeout)) { break; }
    HAL_I2C_Master_Abort_IT(self, addr << 1);

    if /** write */ (wl > 0) {
      if /** write action */
        (HAL_I2C_Master_Seq_Transmit_IT(
           self,
           addr << 1,
           wd,
           wl,
           (rl > 0) ? I2C_LAST_FRAME_NO_STOP : I2C_FIRST_FRAME) != HAL_OK) {
        ho_log("failed=%08X", HAL_I2C_GetError(self));
        ho_event(HO_COMP_I2C,
                 HO_EVT_FAILED,
                 HAL_I2C_Master_Seq_Transmit_IT,
                 (void *) HAL_I2C_GetError(self),
                 (void *) addr,
                 wd,
                 (void *) wl);
        continue;
      }
      while (HAL_I2C_GetState(self) != HAL_I2C_STATE_READY) {}
      if (HAL_I2C_GetError(self) == HAL_I2C_ERROR_AF) {
        ho_event(HO_COMP_I2C, HO_EVT_FAILED, HAL_I2C_Master_Seq_Transmit_IT);
        continue;
      }
    } /** write */

    if /** read */ (rl > 0) {
      if /** read action */
        (HAL_I2C_Master_Seq_Receive_IT(
           self, addr << 1, rd, rl, I2C_FIRST_AND_LAST_FRAME) != HAL_OK) {
        ho_event(HO_COMP_I2C,
                 HO_EVT_FAILED,
                 HAL_I2C_Master_Seq_Receive_IT,
                 (void *) addr,
                 rd,
                 (void *) rl);
        break;
      }
      while (HAL_I2C_GetState(self) != HAL_I2C_STATE_READY) {}
      if (HAL_I2C_GetError(self) == HAL_I2C_ERROR_AF) {
        ho_event(HO_COMP_I2C, HO_EVT_FAILED, HAL_I2C_Master_Seq_Receive_IT);
        continue;
      }
    } /** read */
    return true;
  } while (true);
  ho_event(HO_COMP_I2C, HO_EVT_TIMEOUT, NULL);
  return false;
}

static ho_u32 get_state(void *self HO_UNUSED) { return HAL_I2C_GetState(self); }

static bool is_ready(void *self HO_UNUSED) {
  return HAL_I2C_GetState(self) == HAL_I2C_STATE_READY;
}

struct ho_i2c_interface __ho_stm32_i2c_interface = {
  .ack = ack,
  .transfer = transfer,
  .get_state = get_state,
  .is_ready = is_ready,
};
