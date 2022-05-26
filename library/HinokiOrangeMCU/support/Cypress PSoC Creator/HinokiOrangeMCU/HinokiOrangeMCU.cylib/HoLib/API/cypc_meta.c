/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_meta.h>

/* LINKER ------------------------------------------------------------------- */
void *HO_LINKER_CYPC_META;

/* META --------------------------------------------------------------------- */
const struct __ho_meta_project ho_meta_proj = {
  .basic = { .name = "`$NAME`",
             .version = { .major = `$MAJOR`,
                          .minor = `$MINOR`,
                          .patch = `$PATCH`,
                          .modification = "`$MODIFICATION`" } },
  .rev = "`$REV`"
};
