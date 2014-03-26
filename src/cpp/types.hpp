#ifndef __TYPES_H__
#define __TYPES_H__

#include "arch.hpp"

/** Try to assign type aliases. */
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)

    typedef int bool;
    #define false 0
    #define true 1

    typedef __int8 i8;
    typedef __int16 i16;
    typedef __int32 i32;
    typedef __int64 i64;

    typedef unsigned __int8 u8;
    typedef unsigned __int16 u16;
    typedef unsigned __int32 u32;
    typedef unsigned __int64 u64;

#elif (!defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(LINUX)) \
    || (defined(__INTEL_COMPILER) && !defined(LINUX))

    typedef int bool;
    #define false 0
    #define true 1

    typedef char i8;
    typedef short i16;
    typedef int i32;
    typedef long long i64;

    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long long u64;

#elif defined(LINUX) && defined(__INTEL_COMPILER)

    #include </usr/include/sys/types.h>

    typedef int bool;
    #define false 0
    #define true 1

    typedef char i8;
    typedef short i16;
    typedef int i32;
    typedef long long i64;

    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long long u64;

#else

    #include <stdbool.h>
    #include <stdint.h>

#endif

#if !defined(WORD32) && !defined(WORD64)
#   if defined(ARCH32)
#       define WORD32
#   endif
#   if defined(ARCH64)
#       define WORD64
#   endif
#endif

/** CPU word. */
#if defined(WORD32)
    typedef i32 word_t;
    typedef u32 uword_t;
    typedef i64 dword_t;
    typedef u64 udword_t;
#elif defined(WORD64)
    typedef i64 word_t;
    typedef u64 uword_t;
#   error Undefined 128 bit type.
    // typedef i128 dword_t;
    // typedef u128 udword_t;
#endif

#endif  // __TYPES_H__
