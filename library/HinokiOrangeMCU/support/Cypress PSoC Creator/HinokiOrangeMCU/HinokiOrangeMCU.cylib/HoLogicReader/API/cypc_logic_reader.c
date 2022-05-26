/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include "`$INSTANCE_NAME`_cypc_logic_reader.h"

/* READER ------------------------------------------------------------------- */
static HO_LOGIC_READER_CALLBACK(raw) {
  return `$INPUT_INSTANCE`_Read() > 0 ? HO_HIGH : HO_LOW;
}

struct ho_logic_reader `$INPUT_INSTANCE`_reader = { .read = raw };

/* FILTER ------------------------------------------------------------------- */
#if `$IS_FILTERED`
HO_LOGIC_READER_INSTANCE_FILTER(`$INPUT_INSTANCE`_filtered_reader, &`$INPUT_INSTANCE`_reader,
                                `$RISING_MS`,
                                `$FALLING_MS`);
#endif /** filter */
