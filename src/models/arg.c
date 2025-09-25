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

Clic_Hashmap clic_args_hash(Clic_Arg *standalone_arg, Clic_Arg args[]) {
    Clic_Hashmap map = clic_hashmap_init();

    if (standalone_arg) {
        clic_hashmap_set(&map, standalone_arg->id, standalone_arg);
        clic_hashmap_set(&map, standalone_arg->abbr, standalone_arg);
    }

    if (!args) {
        return map;
    }

    for (int i = 0; i < (sizeof(*args) / sizeof(args[0]) + 1); i++) {
        clic_hashmap_set(&map, args[i].id, &args[i]);
        clic_hashmap_set(&map, args[i].abbr, &args[i]);
    }

    return map;
}

#endif /* __CLIC_MODELS_ARG_C__ */