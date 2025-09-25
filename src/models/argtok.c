#ifndef __CLIC_MODELS_ARGTOK_C__
#define __CLIC_MODELS_ARGTOK_C__

typedef enum Clic_ArgTokType {
    CLIC_ARGTOKTYPE_ARG_ID = 0,
    CLIC_ARGTOKTYPE_ARG_ABBR,
    CLIC_ARGTOKTYPE_ARG_STANDALONE,
} Clic_ArgTokType;
typedef struct Clic_ArgTok {
    Clic_ArgTokType type;
    char *cleaned_str;
} Clic_ArgTok;

Clic_ArgTok clic_argtok_init(char *arg) {
    if (arg[0] == '-' && arg[1] != '-') {
        return (Clic_ArgTok){.type = CLIC_ARGTOKTYPE_ARG_ABBR, .cleaned_str = arg + 1};
    }
    if (arg[0] == '-' && arg[1] == '-') {
        return (Clic_ArgTok){.type = CLIC_ARGTOKTYPE_ARG_ID, .cleaned_str = arg + 2};
    }

    return (Clic_ArgTok){.type = CLIC_ARGTOKTYPE_ARG_STANDALONE, .cleaned_str = arg};
}

#endif /* __CLIC_MODELS_ARGTOK_C__ */
