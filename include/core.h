#ifndef __CLI_CORE_H__

#define __CLI_CORE_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct CliParam CliParam;
typedef struct CliFlag CliFlag;
typedef struct CliCommand CliCommand;

typedef enum CliParamType {
    CLI_BOOL,

    CLI_INT,
    CLI_FLOAT,

    CLI_STR,

    CLI_PATH_FILE,
    CLI_PATH_DIR,
} CliParamType;

typedef struct CliParam {
    char* id;
    CliParamType type;

    bool is_list;
    bool is_nullable;

    char** value_constraints;
    size_t value_constraints_len;

    void* _value;
    size_t _value_list_len;
} CliParam;

typedef struct CliFlag {
    char* id;

    char** aliases;
    size_t aliases_len;

    CliParam* params;
    size_t params_len;

    bool is_nullable;
    bool _is_set;
} CliFlag;

typedef struct CliCommand {
    char* id;
    void* run;

    char** aliases;
    size_t aliases_len;

    CliCommand** cmds;
    size_t cmds_len;

    CliParam* params;
    size_t params_len;

    CliFlag* flags;
    size_t flags_len;

    CliCommand* _next;
    CliCommand* _prev;
} CliCommand;

typedef struct Cli {
    char* name;
    char* epilogue;

    CliCommand** cmds;
    size_t cmds_len;

    CliFlag* flags;
    size_t flags_len;

    CliCommand* _head;
    CliCommand* _tail;
} Cli;

#endif  // !__CLI_CORE_H__