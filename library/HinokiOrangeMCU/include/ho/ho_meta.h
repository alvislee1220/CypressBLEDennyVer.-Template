/* SPDX-License-Identifier: MIT */
#ifndef ___HO_META_H___
#define ___HO_META_H___

/* Inc ---------------------------------------------------------------------- */
#include "ho_typedef.h"

/* Declare ------------------------------------------------------------------ */
extern const struct __ho_meta ho_meta; /*!< Meta Information */

/* Version ------------------------------------------------------------------ */
struct __ho_meta_version {
  const ho_u8 major; /*!< version:major */
  const ho_u8 minor; /*!< version:minor */
  const ho_u8 patch; /*!< version:patch */
  const char *const modification; /*!< version:modification */
};

struct __ho_meta_basic {
  const char *const name; /*!< name */
  const struct __ho_meta_version version; /*!< version */
};

/* Meta --------------------------------------------------------------------- */
struct __ho_meta {
  /** library definition */
  const struct __ho_meta_library {
    const struct __ho_meta_basic basic; /*!< basic metas */
  } library;
  /** project definition */
  const struct __ho_meta_project {
    const struct __ho_meta_basic basic; /*!< basic metas */
    const char *const rev; /*!< project:revision */
  } * project;
};

/* CLI ---------------------------------------------------------------------- */
#include <ho/ho_cli.h>
/* -------------------------------------------------------------------------- */
extern struct ho_cli_cmd ho_cli_cmd_meta;

#endif /* ___HO_META_H___ */
