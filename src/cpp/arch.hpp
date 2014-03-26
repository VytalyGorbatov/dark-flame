#ifndef __ARCH_H__
#define __ARCH_H__

/** Detect arch type and machine bit depth. */
#if defined(i386) || defined(__i386__) || defined(__i386) \
    || defined(_X86_) || defined(__I86__)
#   define X86
#   define ARCH32
#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) \
    || defined(__x86_64) || defined(_M_X64 ) || defined(_M_IX86)
#   define X86
#   define ARCH64
#else
#   error Cannot detect architecture type.
#endif

/** Detect OS. */
#if defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__CYGWIN__)
#   define WINDOWS
#elif defined(__linux__)
#   define LINUX
#endif

/** Detect byte order. */
#if defined(LINUX)
#   include <endian.h>
#endif

#if !defined(LITTLE_ENDIAN) || !defined(BIG_ENDIAN)
#   undef LITTLE_ENDIAN
#   undef BIG_ENDIAN
#   define LITTLE_ENDIAN 1234
#   define BIG_ENDIAN    4321
#endif

#if !defined(BYTE_ORDER) || ((BYTE_ORDER != LITTLE_ENDIAN) && (BYTE_ORDER != BIG_ENDIAN))
#   if defined(X86)
#       define BYTE_ORDER LITTLE_ENDIAN
#   else
#       error Can not determine BYTE_ORDER.
#   endif
#endif

#endif  // __ARCH_H__
