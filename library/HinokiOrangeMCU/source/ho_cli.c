/* SPDX-License-Identifier: MIT */
/* INC ---------------------------------------------------------------------- */
#include <ho/ho_cli.h>
#include <ho/ho_debug.h>
#include <ho/ho_meta.h>
#include <string.h>

/* COMPONENT ---------------------------------------------------------------- */
ho_cstring HO_COMP_CLI = "CLI";

/* DEFAULT ------------------------------------------------------------------ */
HO_CLI_CMD_INSTANCE(ho_cli_cmds_default, NULL, NULL);

/* LOCAL -------------------------------------------------------------------- */
#define LOCK_MASK                                                              \
  (HO_CMD_LOCK(0) | HO_CMD_LOCK(1) | HO_CMD_LOCK(2) | HO_CMD_LOCK(3) |         \
   HO_CMD_LOCK(4) | HO_CMD_LOCK(5) | HO_CMD_LOCK(6) | HO_CMD_LOCK(7))
#define FIRST_PROMPT (1 << 16)
#define ECHO_ENABLE (1 << 17)

/* INPUT PROCESS ------------------------------------------------------------ */
enum input_res {
  INPUT_DATA,
  INPUT_COMPLETE,
  INPUT_BACKSPACE,
  INPUT_UNKNOWN,
};

static inline enum input_res input_process(char c) {
  /** Reference: https://zh.wikipedia.org/wiki/ASCII */
  if ((c >= 32) && (c < 127)) { /** visible char */
    return INPUT_DATA;
  } else if ((c == '\r') || (c == '\n')) { /** CRLF as end */
    return INPUT_COMPLETE;
  } else if /** backspace */ (c == '\b') {
    return INPUT_BACKSPACE;
  } else { /** other */
    return INPUT_UNKNOWN;
  }
}

/* FETCH -------------------------------------------------------------------- */
static inline bool fetch(struct ho_cli_parser *self) {
  char c;
  while (ho_byte_reader_get(self->reader, &c)) {
    switch (input_process(c)) {
    case INPUT_DATA: {
      if (self->buffer.len < (self->buffer.size - 1)) {
        self->buffer.data[self->buffer.len] = c;
        self->buffer.len++;
        if (ho_has_any(self->flag, ECHO_ENABLE)) { ho_cli_put(self, c); }
      }
    } break;
    case INPUT_COMPLETE: {
      self->buffer.data[self->buffer.len] = 0;
      if (ho_has_any(self->flag, ECHO_ENABLE)) { ho_cli_put(self, c); }
    }
      return true;
    case INPUT_UNKNOWN: ho_event(HO_COMP_CLI, HO_EVT_INVALID, self, &c); break;
    case INPUT_BACKSPACE: {
      if (self->buffer.len > 0) {
        self->buffer.len--;
        if (ho_has_any(self->flag, ECHO_ENABLE)) {
          ho_cli_write(self, "\b \b", 3);
        }
      }
    } break;
    }
  }
  return false;
}

/* TOKEN -------------------------------------------------------------------- */
struct token {
  char *ptr;
  ho_u32 len;
};

static inline bool next_token(struct token *self) {
  if (self->ptr == NULL) { return false; }

  /** shift previous */
  self->ptr += self->len;

  /** skip white */
  for (; *self->ptr == ' '; self->ptr++) {}

  /** calculate */
  for (self->len = 0;
       (self->ptr[self->len] != ' ') && (self->ptr[self->len] != 0);
       self->len++) {}

  return self->len > 0;
}

/* UNLOCK? ------------------------------------------------------------------ */
static inline bool is_unlock(struct ho_cli_parser *parser,
                             struct ho_cli_cmd *cmd) {
  return ho_have_all(parser->flag, (cmd->flag & LOCK_MASK));
}

/* MATCH -------------------------------------------------------------------- */
static inline bool is_cmd_matched(struct ho_cli_cmd *cmd, struct token *token) {
  /** token should >= cmd */
  if (cmd->cmd.len > token->len) { return false; }
  /** if not partial, string length should be the same */
  if (!ho_has_any(cmd->flag, HO_CMD_MATCH_PREFIX) &&
      (cmd->cmd.len != token->len)) {
    return false;
  }
  /** compare to match prefix and whole case */
  if (memcmp(cmd->cmd.str, token->ptr, cmd->cmd.len) != 0) { return false; }
  /** match! */
  return true;
}

/* RESET -------------------------------------------------------------------- */
static inline void reset_buffer(struct ho_cli_parser *parser) {
  parser->buffer.len = 0;
}

/* PROMPT ------------------------------------------------------------------- */
static inline void prompt(struct ho_cli_parser *parser) {
  if (parser->prompt.len > 0) {
    ho_cli_write(parser, parser->prompt.str, parser->prompt.len);
  } else /** default */ {
    ho_cli_writef(parser,
                  "%s@%d.%d.%d%s $ ",
                  ho_meta.project->basic.name,
                  ho_meta.project->basic.version.major,
                  ho_meta.project->basic.version.minor,
                  ho_meta.project->basic.version.patch,
                  ho_meta.project->basic.version.modification

    );
  }
}

/* ROUTINE ------------------------------------------------------------------ */
void __ho_cli_parser_routine_callback(struct ho_routine *self) {
  struct ho_cli_parser *parser = self->payload;

  if /** first prompt */ (ho_none_of(parser->flag, FIRST_PROMPT)) {
    reset_buffer(parser);
    prompt(parser);
    ho_mask(parser->flag, FIRST_PROMPT);
  }

  if /** fetch? */ (!fetch(parser)) { return; }

  /** execute commands */ {
    struct ho_cli_cmds *cur = parser->cmds ? parser->cmds->children : NULL;
    struct token token = { .ptr = parser->buffer.data, .len = 0 };
    bool is_matched = false;

    while (cur && next_token(&token)) {
      is_matched = false;
      HO_LIST_STRUCT_FOREACH(cur, cmd) {
        if /** lock? */ (!is_unlock(parser, *cmd)) { continue; }
        if /** match? */ (!is_cmd_matched(*cmd, &token)) { continue; }

        is_matched = true;
        do {
          if /** only compare? */ ((*cmd)->handler == NULL) {
          } else if /** run failed? */ (
            !(*cmd)->handler(*cmd, parser, token.ptr, token.len)) {
            cur = NULL; /** force exit while */
            break;
          } else if /** ok, same layer */ (
            ho_has_any((*cmd)->flag, HO_CMD_BATCH | HO_CMD_PARAM)) {
            break;
          }
          cur = (*cmd)->children;
        } while (0);
        /** token used */ break;
      } /** scan token with command */
      if (!is_matched) {
        token.ptr[token.len] = '\0';
        ho_cli_error(parser, "UnMatch: '%s'", parser->buffer.data);
        break;
      }
    }
  } /** execute */

  /** next prompt */ {
    reset_buffer(parser);
    prompt(parser);
  }
}

/* BUILT-IN ----------------------------------------------------------------- */
HO_CLI_CMD_ADD_WITH_HANDLER(ho_cli_cmds_default, ho_cli_cmd_echo, "echo") {
  if (ho_has_any(parser->flag, ECHO_ENABLE)) {
    ho_cli_ok(parser, "ECHO Disabled");
    ho_unmask(parser->flag, ECHO_ENABLE);
  } else {
    ho_cli_ok(parser, "ECHO Enabled");
    ho_mask(parser->flag, ECHO_ENABLE);
  }
  return true;
}
