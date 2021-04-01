/**
 * DarkFlame Copyright (C) 2014 Alexey Shumeiko
 *
 * This file is part of DarkFlame.
 *
 * DarkFlame is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DarkFlame is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DarkFlame.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For more details see LICENSE file.
 */

#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "arch.hpp"

/** Try to assign type aliases. */
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)

    typedef __int8 int8_t;
    typedef __int16 int16_t;
    typedef __int32 int32_t;
    typedef __int64 int64_t;

    typedef unsigned __int8 uint8_t;
    typedef unsigned __int16 uint16_t;
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t;

#elif (!defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(LINUX) && !defined(__APPLE__)) \
    || (defined(__INTEL_COMPILER) && !defined(LINUX) && !defined(__APPLE__))

    typedef int bool;
    #define false 0
    #define true 1

    typedef char int8_t;
    typedef short int16_t;
    typedef int int32_t;
    typedef long long int64_t;

    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;
    typedef unsigned long long uint64_t;

#elif (defined(LINUX) || defined(__APPLE__)) && defined(__INTEL_COMPILER)

    #include </usr/include/sys/types.h>

    typedef int bool;
    #define false 0
    #define true 1

    typedef char int8_t;
    typedef short int16_t;
    typedef int int32_t;
    typedef long long int64_t;

    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;
    typedef unsigned long long uint64_t;

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
    typedef int32_t word_t;
    typedef uint32_t uword_t;
    typedef int64_t dword_t;
    typedef uint64_t udword_t;
#elif defined(WORD64)
    typedef int64_t word_t;
    typedef uint64_t uword_t;
    // typedef i128 dword_t;
    // typedef u128 udword_t;
#endif

#endif  // __TYPES_HPP__
