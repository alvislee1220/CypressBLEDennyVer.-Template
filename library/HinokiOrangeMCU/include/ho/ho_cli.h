/* SPDX-License-Identifier: MIT */
#ifndef ___HO_CLI_H___
#define ___HO_CLI_H___

/* INC ---------------------------------------------------------------------- */
#include "ho_typedef.h"
#include "ho_byte_interface.h"
#include "ho_list.h"
#include "ho_routine.h"

/* DECLARE ------------------------------------------------------------------ */
struct ho_cli_parser; /*!< Command-line Parser */
struct ho_cli_cmd; /*!< Command-line Command */
struct ho_cli_cmds; /*!< Command List */
/* -------------------------------------------------------------------------- */
extern struct ho_cli_cmd ho_cli_cmds_default;
extern struct ho_cli_cmd ho_cli_cmd_echo;
/* FLAG --------------------------------------------------------------------- */
#define HO_CMD_LOCK(_n) (1U << ((_n) &0x7))
/* -------------------------------- Cmd Only -------------------------------- */
#define HO_CMD_MATCH_PREFIX (1U << 8)
#define HO_CMD_PARAM (1U << 9)
#define HO_CMD_BATCH (1U << 10)
/* REGISTER ----------------------------------------------------------------- */
#define HO_CLI_PARSER_MAKE(_reader, _writer, _size, ...)                       \
  (struct ho_cli_parser) {                                                     \
    .reader = _reader, .writer = _writer,                                      \
    .buffer = { .data = (char[_size]){},                                       \
                .size = _size,                                                 \
                .wdata = (char[_size]){} },                                    \
    .cmds = &ho_cli_cmds_default,                                              \
    .prefix = { .ok = "@OK: ", .error = "@ERROR: " },                          \
    .newline = HO_CSTR_WITH_LEN_MAKE("\r\n"), ##__VA_ARGS__                    \
  }

#define HO_CLI_PARSER_ADD(_parser)                                             \
  HO_LIST_ADD(                                                                 \
    ho_routine,                                                                \
    (&((struct ho_routine){ .callback = __ho_cli_parser_routine_callback,      \
                            .flag = HO_ENABLE,                                 \
                            .payload = (_parser),                              \
                            .name = #_parser })))
#define HO_CLI_PARSER_ANONYMOUS(_reader, _writer, _size, ...)                  \
  HO_CLI_PARSER_ADD(                                                           \
    (&(HO_CLI_PARSER_MAKE(_reader, _writer, _size, ##__VA_ARGS__))))
/* CMD ---------------------------------------------------------------------- */
#define HO_CLI_CMD_CALLBACK(_name)                                             \
  bool _name(struct ho_cli_cmd *cmd HO_UNUSED,                                 \
             struct ho_cli_parser *parser HO_UNUSED,                           \
             const char *raw HO_UNUSED,                                        \
             ho_u32 len HO_UNUSED)
#define HO_CLI_CMD_INSTANCE(_name, _cmd, _handler, ...)                        \
  HO_LIST_INSTANCE(struct ho_cli_cmd, _name);                                  \
  struct ho_cli_cmd _name = (struct ho_cli_cmd){                               \
    .children = (&((struct ho_cli_cmds) HO_LIST_STRUCT_MAKE(_name))),          \
    .handler = _handler,                                                       \
    .cmd = HO_CSTR_WITH_LEN_MAKE(_cmd),                                        \
    ##__VA_ARGS__                                                              \
  };
#define HO_CLI_CMD_INSTANCE_WITH_HANDLER(_name, _cmd, ...)                     \
  static HO_CLI_CMD_CALLBACK(HO_LOCAL_VAR());                                  \
  HO_CLI_CMD_INSTANCE(_name, _cmd, HO_LOCAL_VAR(), ##__VA_ARGS__);             \
  static HO_CLI_CMD_CALLBACK(HO_LOCAL_VAR())
#define HO_CLI_CMD_ADD_WITH_HANDLER(_list, _name, _cmd, ...)                   \
  extern struct ho_cli_cmd _name;                                              \
  HO_LIST_ADD(_list, &_name);                                                  \
  HO_CLI_CMD_INSTANCE_WITH_HANDLER(_name, _cmd, ##__VA_ARGS__)
/* RESPONSE ----------------------------------------------------------------- */
static inline bool ho_cli_put(struct ho_cli_parser *, char);
#define ho_cli_write(_parser, _string, _len)                                   \
  ({ (ho_byte_writer_write((_parser)->writer, _string, _len)); })
#define ho_cli_writef(_parser, _format, ...)                                    \
  ({                                                                            \
    int _len = snprintf(                                                        \
      (_parser)->buffer.wdata, (_parser)->buffer.size, _format, ##__VA_ARGS__); \
    if (_len < 0) { _len = 0; }                                                 \
    (ho_cli_write(_parser, (_parser)->buffer.wdata, _len));                     \
  })
#define ho_cli_writefln(_parser, _format, ...)                                 \
  ({                                                                           \
    (ho_cli_writef(                                                            \
      _parser, _format "%s", ##__VA_ARGS__, (_parser)->newline.str));          \
  })
#define ho_cli_ok(_parser, _format, ...)                                       \
  ({                                                                           \
    (ho_cli_writefln(                                                          \
      _parser, "%s" _format, (_parser)->prefix.ok, ##__VA_ARGS__));            \
  })
#define ho_cli_error(_parser, _format, ...)                                    \
  ({                                                                           \
    (ho_cli_writefln(                                                          \
      _parser, "%s" _format, (_parser)->prefix.error, ##__VA_ARGS__));         \
  })

/* COMPONENT ---------------------------------------------------------------- */
extern ho_cstring HO_COMP_CLI;

/**
 * @brief Command-line Parser
 */
struct ho_cli_parser {
  struct ho_byte_reader *reader;
  struct ho_byte_writer *writer;
  struct __ho_cli_parser_buf {
    char *const data;
    const ho_u32 size;
    ho_u32 len;
    char *const wdata;
  } buffer;
  ho_flag flag;
  struct ho_cli_cmd *const cmds;
  HO_CSTR_WITH_LEN(__ho_cli_prompt) prompt;
  struct __ho_cli_parser_prefix {
    ho_cstring ok;
    ho_cstring error;
  } prefix;
  HO_CSTR_WITH_LEN(__ho_cli_newline) newline;
};
/* IMPL --------------------------------------------------------------------- */
void __ho_cli_parser_routine_callback(struct ho_routine *);
static inline bool ho_cli_put(struct ho_cli_parser *parser, char byte) {
  return ho_byte_writer_put(parser->writer, byte);
}

/**
 * @brief Command
 */
struct ho_cli_cmd {
  HO_CSTR_WITH_LEN() cmd;
  const ho_flag flag;
  bool (*const handler)(struct ho_cli_cmd *,
                        struct ho_cli_parser *,
                        const char *,
                        ho_u32);
  ho_payload payload;
  struct ho_cli_cmds *const children;
};

/**
 * @brief Command list
 */
HO_LIST_STRUCT(struct ho_cli_cmd, ho_cli_cmds);

#endif /* ___HO_CLI_H___ */
