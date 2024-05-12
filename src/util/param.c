#include "../../include/param.h"

CliParam* cli_param_get(CliParam* params, size_t params_len, char* id) {
    for (int i = 0; i < params_len; i++) {
        CliParam* param = &params[i];

        if (strcmp(param->id, id) == 0) {
            return param;
        }
    }

    return NULL;
}

CliParam* cli_param_get_last(CliParam* params, size_t params_len) {
    /** Reach the last param that has not been set */
    while (params->_value) {
        params += 1;
    }

    return params;
}
