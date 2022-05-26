/* SPDX-License-Identifier: MIT */
#ifndef ___`$INSTANCE_NAME`_CYPC_LOGIC_READER_H___
#define ___`$INSTANCE_NAME`_CYPC_LOGIC_READER_H___

/* INC ---------------------------------------------------------------------- */
#include <ho/ho_logic.h>
#include "`$INPUT_INSTANCE`.h"

/* READER ------------------------------------------------------------------- */
extern struct ho_logic_reader `$INPUT_INSTANCE`_reader;

#if `$IS_FILTERED`
extern struct ho_logic_reader `$INPUT_INSTANCE`_filtered_reader;
#endif /** filter */

#endif /* ___`$INSTANCE_NAME`_CYPC_LOGIC_READER_H___ */
