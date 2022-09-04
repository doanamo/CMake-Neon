#pragma once

#if defined(COMPILER_MSVC)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(COMPILER_CLANG)
    #define DEBUG_BREAK() __builtin_trap()
#else
    #error "Unknown compiler!"
#endif

#define DEBUG_ABORT() \
    DEBUG_BREAK(); \
    abort()

#ifdef CONFIG_DEBUG
    #define ASSERT_SIMPLE(exp) \
        if(exp) { } else \
        { \
            LOG_CRITICAL("Assertion failed: " STRINGIFY(exp)); \
            DEBUG_ABORT(); \
        }

    #define ASSERT_MESSAGE(exp, msg) \
        if(exp) { } else \
        { \
            LOG_CRITICAL("Assertion failed: " STRINGIFY(exp) " - " msg); \
            DEBUG_ABORT(); \
        }

    #define ASSERT_FORMAT(exp, msg, ...) \
        if(exp) { } else \
        { \
            LOG_CRITICAL("Assertion failed: " STRINGIFY(exp) " - " msg, ## __VA_ARGS__); \
            DEBUG_ABORT(); \
        }

    #define ASSERT_DEDUCE(arg1, arg2, arg3, arg4, arg5, arg6, arg7, ...) arg7
    #define ASSERT_CHOOSER(...) EXPAND(ASSERT_DEDUCE(__VA_ARGS__, ASSERT_FORMAT, \
        ASSERT_FORMAT, ASSERT_FORMAT, ASSERT_FORMAT, ASSERT_MESSAGE, ASSERT_SIMPLE))
    #define ASSERT(...) EXPAND(ASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__))
#else
    #define ASSERT(exp, ...) ((void)0)
#endif
