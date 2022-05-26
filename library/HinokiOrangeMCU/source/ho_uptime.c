/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_uptime.h>
#include <ho/ho_ticker.h>
#include <ho/ho_routine.h>
#include <ho/ho_countdown.h>

/* LOCAL -------------------------------------------------------------------- */
static const char name[] = "Uptime";

/* IMPL --------------------------------------------------------------------- */
struct __ho_uptime ho_uptime = { 0 };

/* TICKER ------------------------------------------------------------------- */
HO_TICKER_ANONYMOUS() { ho_uptime.msec++; }

/* ROUTINE ------------------------------------------------------------------ */
HO_ROUTINE_ANONYMOUS(.name = name) {
  while (ho_uptime.msec >= 1000) {
    ho_uptime.msec -= 1000;
    ho_uptime.sec++;
  }

  if (ho_uptime.sec == 60) {
    ho_uptime.sec = 0;
    ho_uptime.min++;
  }

  if (ho_uptime.min == 60) {
    ho_uptime.min = 0;
    ho_uptime.hour++;
  }

  if (ho_uptime.hour == 24) {
    ho_uptime.hour = 0;
    ho_uptime.day++;
  }
}
