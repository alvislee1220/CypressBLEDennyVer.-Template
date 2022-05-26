/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_logic_led.h>

/* IMPL --------------------------------------------------------------------- */
HO_TICKER_CALLBACK(__ho_logic_led_ticker_callback) {
  struct ho_logic_led *led = ticker->payload;
  if (led->_count < led->brightness) {
    ho_logic_write(led->led, led->active);
  } else {
    ho_logic_write(led->led, led->active == HO_HIGH ? HO_LOW : HO_HIGH);
  }
  led->_count++;
  if (led->_count == led->step) { led->_count = 0; }
}
