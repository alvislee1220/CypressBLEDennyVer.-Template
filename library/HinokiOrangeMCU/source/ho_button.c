/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_button.h>

/* LOCAL -------------------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_btn, ho_btn);

/* IMPL --------------------------------------------------------------------- */
ho_cstring HO_COMP_BUTTON = "Button";

/* API ---------------------------------------------------------------------- */
bool ho_btn_enable(struct ho_btn *self) {
  if (ho_has_any(self->flag, HO_ENABLE)) { return false; }
  ho_event(HO_COMP_BUTTON, HO_EVT_ENABLE, self);
  HO_CRITICAL_AREA() {
    ho_btn_lock(self);
    ho_mask(self->flag, HO_ENABLE);
  }
  return true;
}

bool ho_btn_disable(struct ho_btn *self) {
  if (ho_none_of(self->flag, HO_ENABLE)) { return false; }
  ho_event(HO_COMP_BUTTON, HO_EVT_DISABLE, self);
  HO_CRITICAL_AREA() {
    ho_btn_lock(self);
    ho_unmask(self->flag, HO_ENABLE);
  }
  return true;
}

/* ROUTINE ------------------------------------------------------------------ */
void __ho_btn_routine_callback(struct ho_routine *self) {
  struct ho_btn *btn = self->payload;
  /** enabled? */
  if (ho_none_of(btn->flag, HO_ENABLE)) { return; }

  /** freeze read */
  enum ho_logic logic = ho_logic_read(btn->input);
  bool is_actived = (logic == btn->active);

  /** locked? */
  if (ho_has_any(btn->flag, HO_BTN_LOCK)) {
    if (is_actived) { return; } /** still locked */
    /** unlock */
    ho_btn_reset(btn);
    ho_btn_unlock(btn);
  }

  /** pressed/holding */
  if (is_actived) {
    /** into pressed? */
    if (ho_none_of(btn->flag, HO_BTN_PRESS)) {
      ho_mask(btn->flag, HO_BTN_PRESS);
      ho_unmask(btn->flag, HO_BTN_RELEASE);
      btn->stamp.pressed = ho_get_tick();
      btn->repeat.ts = ho_get_tick();
      if (ho_has_any(btn->flag, HO_BTN_CLICK_COUNT)) {
        btn->click.times = 0;
        ho_unmask(btn->flag, HO_BTN_CLICK_COUNT);
      }
    }
  }
  /** release or idle */
  else {
    /** into release = from press to release */
    if (ho_has_any(btn->flag, HO_BTN_PRESS)) {
      btn->stamp.released = ho_get_tick();
      ho_mask(btn->flag, HO_BTN_RELEASE);
      ho_unmask(btn->flag, HO_BTN_PRESS);
      btn->click.times++;
    }

    /** click timeout */
    if (ho_has_any(btn->flag, HO_BTN_RELEASE) &&
        ho_none_of(btn->flag, HO_BTN_CLICK_COUNT) &&
        ho_is_ticks_up(btn->stamp.released, btn->click.timeout)) {
      ho_mask(btn->flag, HO_BTN_CLICK_COUNT);
    }
  }
}
