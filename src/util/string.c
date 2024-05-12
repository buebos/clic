static const unsigned char STR_UPPER_START = 65;
static const unsigned char STR_UPPER_END = 90;

static const unsigned char STR_UPPER_LOWER_GAP = 32;

/**
 * @brief Overrides the string to replace upper case
 * chars with lowe case chars.
 * 
 * @param str 
 */
void str_low(char* str) {
    while (*str) {
        if (
            *str <= STR_UPPER_END &&
            *str >= STR_UPPER_START

        ) {
            *str += STR_UPPER_LOWER_GAP;
        }

        str += 1;
    }
}