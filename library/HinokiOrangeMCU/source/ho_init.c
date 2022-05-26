/* SPDX-License-Identifier: MIT */
/* Inc ---------------------------------------------------------------------- */
#include <ho/ho_init.h>

/* Debug -------------------------------------------------------------------- */
ho_cstring HO_COMP_INIT = "Init";
ho_cstring HO_COMP_INIT_STAGE = "InitStage";

/* Initialize After Reset --------------------------------------------------- */
HO_LIST_INSTANCE(ho_init_after_reset_callback, ho_init_after_reset_callback);

/* Initialize Stage --------------------------------------------------------- */
#define STAGE_INSTANCE(_stage, _order)                                         \
  HO_LIST_INSTANCE(struct ho_init, _stage);                                    \
  struct ho_init_stage _stage = HO_LIST_STRUCT_MAKE(_stage);                   \
  HO_LIST_ADD(__ho_init_list, &_stage, #_order);

STAGE_INSTANCE(ho_init_clock, 1);
STAGE_INSTANCE(ho_init_peripheral, 2);
STAGE_INSTANCE(ho_init_library, 3);
STAGE_INSTANCE(ho_init_service, 4);
STAGE_INSTANCE(ho_init_app, 5);

/* Init --------------------------------------------------------------------- */
HO_LIST_INSTANCE(struct ho_init_stage, __ho_init_list);
