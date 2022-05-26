/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "`$INSTANCE_NAME`_cypc_logic.h"

/* IMPL --------------------------------------------------------------------- */
HO_LOGIC_READER_CALLBACK(cypc) { return ho_cypc_pin_read(payload); }
