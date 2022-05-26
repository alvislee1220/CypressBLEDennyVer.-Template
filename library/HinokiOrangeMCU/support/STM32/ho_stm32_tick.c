/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include HO_STM32_HEADER
#include <ho/ho_ticker.h>

void HAL_IncTick(void) {
  uwTick += (uint32_t) uwTickFreq;
  ho_process_ticker();
}
