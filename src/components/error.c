#ifndef __CLIC_COMPONENTS_RESULT_C__
#define __CLIC_COMPONENTS_RESULT_C__

#include <stdio.h>

typedef struct Clic_Error {
    int code;
    char* message;
} Clic_Error;

int clic_error_print(Clic_Error* error, char* override_message) {
    char* message = NULL;
    char buffer[16] = {0};

    if (error->message && error->message[0]) {
        message = error->message;
    } else if (override_message && override_message[0]) {
        message = override_message;
    } else {
        snprintf(
            buffer,
            sizeof(buffer),
            "Code: %d. No message provided.",
            error->code

        );

        message = buffer;
    }

    printf("[ERROR]: %s\n", message);

    return error->code;
}
/*
 * CLIC ERROR HANDLING MACROS
 *
 * These macros automatically check Clic_Error return values and call
 * clic_error_print() + return on error. Use in functions that return int.
 */

/* Option 1: Dynamic (expr)(expr) syntax - unlimited expressions
 * Usage:
 *   CLIC_ERROR_HANDLE((func1())(func2())(func3()))
 */
#define _clic_error_handle_one(expr)              \
    {                                             \
        Clic_Error _err = (expr);                 \
        if (_err.code != 0)                       \
            return clic_error_print(&_err, NULL); \
    }

#define _clic_error_loop(seq) _clic_error_loop_end(_clic_error_loop_a seq)
#define _clic_error_loop_a(x)  \
    _clic_error_handle_one(x); \
    _clic_error_loop_b
#define _clic_error_loop_b(x)  \
    _clic_error_handle_one(x); \
    _clic_error_loop_a
#define _clic_error_loop_a_end
#define _clic_error_loop_b_end
#define _clic_error_loop_end(...) _clic_error_loop_end_(__VA_ARGS__)
#define _clic_error_loop_end_(...) __VA_ARGS__##_end

#define CLIC_ERROR_HANDLE(seq) _clic_error_loop(seq)

/* Option 2: Block-style syntax - unlimited expressions, cleanest
 * Usage:
 *   CLIC_ERROR_CHECK_BEGIN()
 *       CLIC_ERROR_CHECK(func1());
 *       CLIC_ERROR_CHECK(func2());
 *   CLIC_ERROR_CHECK_END()
 */
#define CLIC_ERROR_CHECK_BEGIN() {
#define CLIC_ERROR_CHECK(expr)                    \
    {                                             \
        Clic_Error _err = (expr);                 \
        if (_err.code != 0)                       \
            return clic_error_print(&_err, NULL); \
    }
#define CLIC_ERROR_CHECK_END() }

/**
 * Option 3: Comma-separated syntax - up to 8 expressions, most natural
 * Usage:
 *   CLIC_ERROR_CHECK_MULTI(func1(), func2(), func3());
 * NOTE: Limited to 8 expressions max. Extend _GET_MACRO_8 for more.
 */
#define _CLIC_ERROR_CHECK_ONE(expr)               \
    {                                             \
        Clic_Error _err = (expr);                 \
        if (_err.code != 0)                       \
            return clic_error_print(&_err, NULL); \
    }

#define CLIC_ERROR_CHECK_MULTI_1(a) _CLIC_ERROR_CHECK_ONE(a)
#define CLIC_ERROR_CHECK_MULTI_2(a, b) \
    _CLIC_ERROR_CHECK_ONE(a);          \
    _CLIC_ERROR_CHECK_ONE(b)
#define CLIC_ERROR_CHECK_MULTI_3(a, b, c) \
    _CLIC_ERROR_CHECK_ONE(a);             \
    _CLIC_ERROR_CHECK_ONE(b);             \
    _CLIC_ERROR_CHECK_ONE(c)
#define CLIC_ERROR_CHECK_MULTI_4(a, b, c, d) \
    _CLIC_ERROR_CHECK_ONE(a);                \
    _CLIC_ERROR_CHECK_ONE(b);                \
    _CLIC_ERROR_CHECK_ONE(c);                \
    _CLIC_ERROR_CHECK_ONE(d)
#define CLIC_ERROR_CHECK_MULTI_5(a, b, c, d, e) \
    _CLIC_ERROR_CHECK_ONE(a);                   \
    _CLIC_ERROR_CHECK_ONE(b);                   \
    _CLIC_ERROR_CHECK_ONE(c);                   \
    _CLIC_ERROR_CHECK_ONE(d);                   \
    _CLIC_ERROR_CHECK_ONE(e)
#define CLIC_ERROR_CHECK_MULTI_6(a, b, c, d, e, f) \
    _CLIC_ERROR_CHECK_ONE(a);                      \
    _CLIC_ERROR_CHECK_ONE(b);                      \
    _CLIC_ERROR_CHECK_ONE(c);                      \
    _CLIC_ERROR_CHECK_ONE(d);                      \
    _CLIC_ERROR_CHECK_ONE(e);                      \
    _CLIC_ERROR_CHECK_ONE(f)
#define CLIC_ERROR_CHECK_MULTI_7(a, b, c, d, e, f, g) \
    _CLIC_ERROR_CHECK_ONE(a);                         \
    _CLIC_ERROR_CHECK_ONE(b);                         \
    _CLIC_ERROR_CHECK_ONE(c);                         \
    _CLIC_ERROR_CHECK_ONE(d);                         \
    _CLIC_ERROR_CHECK_ONE(e);                         \
    _CLIC_ERROR_CHECK_ONE(f);                         \
    _CLIC_ERROR_CHECK_ONE(g)
#define CLIC_ERROR_CHECK_MULTI_8(a, b, c, d, e, f, g, h) \
    _CLIC_ERROR_CHECK_ONE(a);                            \
    _CLIC_ERROR_CHECK_ONE(b);                            \
    _CLIC_ERROR_CHECK_ONE(c);                            \
    _CLIC_ERROR_CHECK_ONE(d);                            \
    _CLIC_ERROR_CHECK_ONE(e);                            \
    _CLIC_ERROR_CHECK_ONE(f);                            \
    _CLIC_ERROR_CHECK_ONE(g);                            \
    _CLIC_ERROR_CHECK_ONE(h)

#define _GET_MACRO_8(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define CLIC_ERROR_CHECK_MULTI(...) \
    _GET_MACRO_8(__VA_ARGS__, CLIC_ERROR_CHECK_MULTI_8, CLIC_ERROR_CHECK_MULTI_7, CLIC_ERROR_CHECK_MULTI_6, CLIC_ERROR_CHECK_MULTI_5, CLIC_ERROR_CHECK_MULTI_4, CLIC_ERROR_CHECK_MULTI_3, CLIC_ERROR_CHECK_MULTI_2, CLIC_ERROR_CHECK_MULTI_1)(__VA_ARGS__)

/* Option 4: Statement expressions - GCC/Clang only, one per line
 * Usage:
 *   CLIC_ERROR_TRY(func1());
 *   CLIC_ERROR_TRY(func2());
 * NOTE: GNU C extension, won't work with MSVC
 */
#ifdef __GNUC__
#define CLIC_ERROR_TRY(expr)                      \
    ({                                            \
        Clic_Error _err = (expr);                 \
        if (_err.code != 0)                       \
            return clic_error_print(&_err, NULL); \
        _err;                                     \
    })
#endif

/* Option 5: Single statement macro - simple one-liner checks
 * Usage:
 *   CLIC_ERROR_CHECK_ONE(func1());
 *   CLIC_ERROR_CHECK_ONE(func2());
 * NOTE: Same as CLIC_ERROR_CHECK in Option 2, but standalone
 */
#define CLIC_ERROR_CHECK_ONE(expr)                \
    {                                             \
        Clic_Error _err = (expr);                 \
        if (_err.code != 0)                       \
            return clic_error_print(&_err, NULL); \
    }

#endif /* __CLIC_COMPONENTS_RESULT_C__ */