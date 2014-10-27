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

#ifndef __ARCH_HPP__
#define __ARCH_HPP__

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

#endif  // __ARCH_HPP__
