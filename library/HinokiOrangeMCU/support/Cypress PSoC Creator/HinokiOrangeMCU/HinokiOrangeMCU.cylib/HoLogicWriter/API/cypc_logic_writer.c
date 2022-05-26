/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "`$INSTANCE_NAME`_cypc_logic_writer.h"

static HO_LOGIC_WRITER_CALLBACK(write) { `$OUTPUT_INSTANCE`_Write(logic); }

struct ho_logic_writer `$OUTPUT_INSTANCE`_writer = { .write = write };
