#ifndef __CLIC_MODELS_ARG_C__
#define __CLIC_MODELS_ARG_C__

#include <stdbool.h>
#include <stdlib.h>

#include "../components/error.c"
#include "../components/hashmap.c"

typedef enum Clic_ArgType {
    CLIC_ARGTYPE_BOOLEAN = 0,
    CLIC_ARGTYPE_INT,
    CLIC_ARGTYPE_FLOAT,
    CLIC_ARGTYPE_STRING,
    CLIC_ARGTYPE_ENUM,
} Clic_ArgType;

typedef struct Clic_ArgConstraintBool {
    bool value;
} Clic_ArgConstraintBool;
typedef struct Clic_ArgConstraintInteger {
    int min;
    int max;
    int divisible;

    int value;
} Clic_ArgConstraintInteger;
typedef struct Clic_ArgConstraintFloat {
    float min;
    float max;

    float value;
} Clic_ArgConstraintFloat;
typedef struct Clic_ArgConstraintString {
    size_t min_len;
    size_t max_len;

    char *value;
} Clic_ArgConstraintString;
typedef struct Clic_ArgConstraintEnum {
    char **values;
    char *value;
} Clic_ArgConstraintEnum;

typedef union Clic_ArgConstraint {
    Clic_ArgConstraintInteger integer;
    Clic_ArgConstraintFloat floatingpoint;
    Clic_ArgConstraintString string;
    Clic_ArgConstraintEnum enumeration;
    Clic_ArgConstraintBool boolean;
} Clic_ArgConstraint;

typedef struct Clic_Arg {
    char *id;
    char *abbr;
    char *description;

    bool nullable;

    Clic_ArgType type;
    Clic_ArgConstraint constraint;

    /* Private */
    struct _Clic_Arg {
        bool passed_in;
        bool standalone;
    } _;
} Clic_Arg;

typedef enum Clic_ArgvType {
    CLIC_ARGVTYPE_ARG_ID = 0,
    CLIC_ARGVTYPE_ARG_ABBR,
    CLIC_ARGVTYPE_ARG_STANDALONE,
} Clic_ArgvType;
typedef struct Clic_Argv {
    Clic_ArgvType type;
    char *cleaned_str;
} Clic_Argv;

Clic_Argv clic_argv_init(char *argv) {
    if (argv[0] == '-' && argv[1] != '-') {
        return (Clic_Argv){.type = CLIC_ARGVTYPE_ARG_ABBR, .cleaned_str = argv + 1};
    }
    if (argv[0] == '-' && argv[1] == '-') {
        return (Clic_Argv){.type = CLIC_ARGVTYPE_ARG_ID, .cleaned_str = argv + 2};
    }

    return (Clic_Argv){.type = CLIC_ARGVTYPE_ARG_STANDALONE, .cleaned_str = argv};
}

Clic_Hashmap clic_args_hash(Clic_Arg *standalone_arg, Clic_Arg args[]) {
    Clic_Hashmap map = clic_hashmap_init();

    clic_hashmap_set(&map, standalone_arg->id, standalone_arg);
    clic_hashmap_set(&map, standalone_arg->abbr, standalone_arg);

    for (int i = 0; i < (sizeof(*args) / sizeof(args[0]) + 1); i++) {
        clic_hashmap_set(&map, args[i].id, &args[i]);
        clic_hashmap_set(&map, args[i].abbr, &args[i]);
    }
    return map;
}

Clic_Error clic_args_parse(
    Clic_Arg *standalone_arg,
    Clic_Arg *args,
    Clic_Hashmap *args_hash,
    int argc,
    char *argv[]

) {
    for (int i = 0; i < argc; i++) {
        Clic_Argv argvelem = clic_argv_init(argv[i]);

        Clic_Arg *arg_target = NULL;
        char *arg_str_value = NULL;

        if (argvelem.type == CLIC_ARGVTYPE_ARG_STANDALONE) {
            arg_target = standalone_arg;
        }
        if (argvelem.type == CLIC_ARGVTYPE_ARG_ID || argvelem.type == CLIC_ARGVTYPE_ARG_ABBR) {
            arg_target = clic_hashmap_get(args_hash, argvelem.cleaned_str);
        }

        if (NULL == arg_target) {
            return (Clic_Error){.code = -1, .message = "No argument target found."};
        }

        if (argvelem.type == CLIC_ARGVTYPE_ARG_STANDALONE) {
            arg_str_value = argvelem.cleaned_str;
        } else if (arg_target->type != CLIC_ARGTYPE_BOOLEAN) {
            if (i == argc - 1 && arg_target->nullable == false) {
                return (Clic_Error){.code = -1, .message = "No argument value provided."};
            }

            Clic_Argv argvelem_next = clic_argv_init(argv[i + 1]);

            if (argvelem_next.type != CLIC_ARGVTYPE_ARG_STANDALONE) {
                return (Clic_Error){.code = -1, .message = "No argument value provided."};
            }

            arg_str_value = argvelem_next.cleaned_str;
            i++;
        }

        switch (arg_target->type) {
            case CLIC_ARGTYPE_BOOLEAN:
                arg_target->constraint.boolean.value = true;
                break;
            case CLIC_ARGTYPE_INT:
                arg_target->constraint.integer.value = atoi(arg_str_value);
                break;
            case CLIC_ARGTYPE_FLOAT:
                arg_target->constraint.floatingpoint.value = atof(arg_str_value);
                break;
            case CLIC_ARGTYPE_STRING:
                arg_target->constraint.string.value = arg_str_value;
                break;
            case CLIC_ARGTYPE_ENUM:
                arg_target->constraint.enumeration.value = arg_str_value;
                break;
            default:
                break;
        }
    }

    return (Clic_Error){0};
}

#endif /* __CLIC_MODELS_ARG_C__ */