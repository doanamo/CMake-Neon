#pragma once

#if defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(__clang__)
    #define COMPILER_CLANG
#else
    #error "Unknown compiler!"
#endif

#if defined(NDEBUG)
    #define CONFIG_RELEASE
#else
    #define CONFIG_DEBUG
#endif

#define EXPAND(x) x
#define STRINGIFY(x) #x
#define PRAGMA(x) _Pragma(#x)
#define UNUSED(x) (void)(x)

#if defined(COMPILER_MSVC)
    #define COMPILER_WARNINGS_PUSH() PRAGMA(warning(push))
    #define COMPILER_WARNINGS_POP() PRAGMA(warning(pop))
    #define COMPILER_WARNINGS_DISABLE_ALL_PUSH() PRAGMA(warning(push, 0))
    #define COMPILER_WARNINGS_DISABLE_ALL_POP() PRAGMA(warning(pop))
#elif defined(COMPILER_CLANG)
    #define COMPILER_WARNINGS_PUSH() PRAGMA(clang diagnostic push)
    #define COMPILER_WARNINGS_POP() PRAGMA(clang diagnostic pop)
    #define COMPILER_WARNINGS_DISABLE_ALL_PUSH() \
        PRAGMA(clang diagnostic push) \
        PRAGMA(clang diagnostic ignored "-Weverything")
    #define COMPILER_WARNINGS_DISABLE_ALL_POP() \
        PRAGMA(clang diagnostic pop)
#endif

#if defined(COMPILER_MSVC)
    #define MSVC_WARNING_DISABLE(id) PRAGMA(warning(disable:id))
#else
    #define MSVC_WARNING_DISABLE(id)
#endif

#if defined(COMPILER_CLANG)
    #define CLANG_WARNING_DISABLE(name) PRAGMA(clang diagnostic ignored name)
#else
    #define CLANG_WARNING_DISABLE(name)
#endif
