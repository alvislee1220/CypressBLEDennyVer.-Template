/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_logic.h>

/* INJECTABLE --------------------------------------------------------------- */
HO_LOGIC_READER_CALLBACK(__ho_logic_reader_injectable_callback) {
  return *(enum ho_logic *) payload;
}

/* FILTER ------------------------------------------------------------------- */
HO_LOGIC_READER_CALLBACK(__ho_logic_reader_filter_callback) {
  return ((struct __ho_logic_filter *) (payload))->logic;
}
/* -------------------------------------------------------------------------- */
#define FIRST_SCAN (1 << 16)

HO_ROUTINE_CALLBACK(__ho_logic_filter_routine_callback) {
  struct ho_logic_reader *reader = routine->payload;
  struct __ho_logic_filter *filter = reader->payload;
  struct __ho_logic_filter_rt *rt = &filter->filter;
  enum ho_logic cur = ho_logic_filter_raw(reader);

  /** first scan */
  if (ho_none_of(routine->flag, FIRST_SCAN)) {
    filter->logic = cur;
    ho_mask(routine->flag, FIRST_SCAN);
  }

  /** not change, or resume */
  if (cur == filter->logic) {
    rt->_ts = ho_get_tick();
    return;
  }

  do {
    if ((cur == HO_HIGH) && ho_is_ticks_up(rt->_ts, rt->rising)) { break; }
    if ((cur == HO_LOW) && ho_is_ticks_up(rt->_ts, rt->falling)) { break; }
    /** not pass jitter time */
    return;
  } while (0);

  /** logic change */
  filter->logic = cur;
}
