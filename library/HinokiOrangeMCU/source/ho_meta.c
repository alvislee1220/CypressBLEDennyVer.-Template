/* SPDX-License-Identifier: MIT */

/* Library ------------------------------------------------------------------ */
#define LIB_NAME "HinokiOrangeMCU"
#define LIB_VER_MAJOR 1
#define LIB_VER_MINOR 2
#define LIB_VER_PATCH 2
#define LIB_VER_MOD ""

/* Inc ---------------------------------------------------------------------- */
#include <ho/ho_meta.h>

/* DEFAULT ------------------------------------------------------------------ */
#if !defined(HO_PROJ_NAME)
#define HO_PROJ_NAME "Project"
#endif /* HO_PROJ_NAME */

#if !defined(HO_PROJ_REV)
#define HO_PROJ_REV "Revision"
#endif /* HO_PROJ_REV */

#if !defined(HO_PROJ_VER_MAJOR)
#define HO_PROJ_VER_MAJOR 1
#endif /* HO_PROJ_VER_MAJOR */

#if !defined(HO_PROJ_VER_MINOR)
#define HO_PROJ_VER_MINOR 0
#endif /* HO_PROJ_VER_MINOR */

#if !defined(HO_PROJ_VER_PATCH)
#define HO_PROJ_VER_PATCH 0
#endif /* HO_PROJ_VER_PATCH */

#if !defined(HO_PROJ_VER_MOD)
#define HO_PROJ_VER_MOD ""
#endif /* HO_PROJ_VER_MOD */

const struct __ho_meta_project ho_meta_proj HO_WEAK = {
  .basic = { .name = HO_PROJ_NAME,
             .version = { .major = HO_PROJ_VER_MAJOR,
                          .minor = HO_PROJ_VER_MINOR,
                          .patch = HO_PROJ_VER_PATCH,
                          .modification = HO_PROJ_VER_MOD } },
  .rev = HO_PROJ_REV
};

/* Meta --------------------------------------------------------------------- */
const struct __ho_meta ho_meta = {
  .library = { .basic = { .name = LIB_NAME,
                          .version = { .major = LIB_VER_MAJOR,
                                       .minor = LIB_VER_MINOR,
                                       .patch = LIB_VER_PATCH,
                                       .modification = LIB_VER_MOD } } },
  .project = &ho_meta_proj
};

/* INIT --------------------------------------------------------------------- */
#include <ho/ho_init.h>
HO_INIT_ANONYMOUS(ho_init_app) {
  ho_log(HO_STRING_CLEAR_TERMINAL "!!! %s (%s:%d.%d.%d%s)",
         ho_meta.project->basic.name,
         ho_meta.project->rev,
         ho_meta.project->basic.version.major,
         ho_meta.project->basic.version.minor,
         ho_meta.project->basic.version.patch,
         ho_meta.project->basic.version.modification);
}

/* CMDS --------------------------------------------------------------------- */
#include <ho/ho_cli.h>
#include <string.h>
/* --------------------------------- Common --------------------------------- */
static const struct __ho_meta_basic *cmd_basic = NULL;
HO_CLI_CMD_CALLBACK(cmd_meta_proj_cb) {
  cmd_basic = &ho_meta.project->basic;
  return true;
}

/* ---------------------------------- Meta ---------------------------------- */
HO_CLI_CMD_INSTANCE(ho_cli_cmd_meta, "meta", cmd_meta_proj_cb);
HO_LIST_ADD(ho_cli_cmds_default, &ho_cli_cmd_meta);
/* ------------------------------ Meta Project ------------------------------ */
HO_CLI_CMD_INSTANCE(cmd_proj, "-proj", cmd_meta_proj_cb, .flag = HO_CMD_PARAM);
HO_LIST_ADD(ho_cli_cmd_meta, &cmd_proj);
/* -------------------------------- Meta -lib ------------------------------- */
HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmd_meta,
                            cmd_lib,
                            "-lib",
                            .flag = HO_CMD_PARAM) {
  cmd_basic = &ho_meta.library.basic;
  return true;
}
/* -------------------------------- Meta Name ------------------------------- */
HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmd_meta,
                            cmd_name,
                            "name",
                            .flag = HO_CMD_BATCH) {
  ho_cli_ok(parser, "%s", cmd_basic->name);
  return true;
}

/* ------------------------------ Meta Version ------------------------------ */
HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmd_meta,
                            cmd_ver,
                            "ver",
                            .flag = HO_CMD_BATCH | HO_CMD_MATCH_PREFIX) {
  do {
    if (HO_CSTRING_CMP(raw, len, "ver")) { break; }
    if (HO_CSTRING_CMP(raw, len, "version")) { break; }
    ho_cli_error(parser, "%s not accept", raw);
    return false;
  } while (0);

  ho_cli_ok(parser,
            "%d.%d.%d%s",
            cmd_basic->version.major,
            cmd_basic->version.minor,
            cmd_basic->version.patch,
            cmd_basic->version.modification);
  return true;
}

/* ------------------------------ Meta Revision ----------------------------- */
HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmd_meta,
                            cmd_rev,
                            "rev",
                            .flag = HO_CMD_BATCH) {
  if (cmd_basic == &ho_meta.library.basic) {
    ho_cli_error(parser, "Library no revision");
    return false;
  }
  ho_cli_ok(parser, "%s", ho_meta.project->rev);
  return true;
}

/* LINKER ------------------------------------------------------------------- */
void *HO_LINKER_META;
