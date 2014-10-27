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

#ifndef __BYTESWAP_HPP__
#define __BYTESWAP_HPP__

#include <string.h>

#include "arch.hpp"
#include "types.hpp"

/* Use align?. */
#ifndef NEED_ALIGN
#   if defined(ARM)
#       define NEED_ALIGN
#   endif
#endif

#define SRC_UNALIGNED_ (1 << 0)
#define DST_UNALIGNED_ (1 << 1)

#ifdef NEED_ALIGN
#   define SRC_UNALIGNED SRC_UNALIGNED_
#   define DST_UNALIGNED DST_UNALIGNED_
#else
#   define SRC_UNALIGNED 0
#   define DST_UNALIGNED 0
#endif

#define swap_bytes(src, dst, size)              \
    {                                           \
        size_t _size = size;                    \
        u8* _src_start = (u8*)(src);            \
        u8* _src_end = (u8*)(src) + _size - 1;  \
        u8* _dst_start = (u8*)(dst);            \
        u8* _dst_end = (u8*)(dst) + _size - 1;  \
        while (_src_start <= _src_end) {        \
            u8 _tmp = *_src_start++;            \
            *_dst_start++ = *_src_end--;        \
            *_dst_end-- = _tmp;                 \
        }                                       \
    }

#define le_to_be(src, dst, size) swap_bytes(src, dst, size)
#define be_to_le(src, dst, size) swap_bytes(src, dst, size)

#define copy_word32(src, dst, flag)             \
    {                                           \
        switch(flag) {                          \
        case 0:                                 \
        {                                       \
            u32* _src = (u32*)(src);            \
            u32* _dst = (u32*)(dst);            \
            *_dst = *_src;                      \
        }                                       \
        break;                                  \
        case SRC_UNALIGNED_:                    \
        case DST_UNALIGNED_:                    \
        case SRC_UNALIGNED_ | DST_UNALIGNED_:   \
        default:                                \
        {                                       \
            u8* _src = (u8*)(src);              \
            u8* _dst = (u8*)(dst);              \
            _dst[0] = _src[0];                  \
            _dst[1] = _src[1];                  \
            _dst[2] = _src[2];                  \
            _dst[3] = _src[3];                  \
        }                                       \
        }                                       \
    }

#define copy_word64(src, dst, flag)             \
    {                                           \
        switch(flag) {                          \
        case 0:                                 \
        {                                       \
            u64* _src = (u64*)(src);            \
            u64* _dst = (u64*)(dst);            \
            *_dst = *_src;                      \
        }                                       \
        break;                                  \
        case SRC_UNALIGNED_:                    \
        case DST_UNALIGNED_:                    \
        case SRC_UNALIGNED_ | DST_UNALIGNED_:   \
        default:                                \
        {                                       \
            u8* _src = (u8*)(src);              \
            u8* _dst = (u8*)(dst);              \
            _dst[0] = _src[0];                  \
            _dst[1] = _src[1];                  \
            _dst[2] = _src[2];                  \
            _dst[3] = _src[3];                  \
            _dst[4] = _src[4];                  \
            _dst[5] = _src[5];                  \
            _dst[6] = _src[6];                  \
            _dst[7] = _src[7];                  \
        }                                       \
        }                                       \
    }

#define swap_word32(src, dst, flag)                 \
    {                                               \
        switch (flag) {                             \
        case 0:                                     \
        {                                           \
            u32* _src = (u32*)(src);                \
            u32* _dst = (u32*)(dst);                \
            *_dst = ((*_src >> 24) & 0xff)          \
                    | ((*_src >> 8) & 0xff00)       \
                    | ((*_src <<  8) & 0xff0000)    \
                    | ( *_src << 24);               \
        }                                           \
        break;                                      \
        case SRC_UNALIGNED_:                        \
        case DST_UNALIGNED_:                        \
        case SRC_UNALIGNED_ | DST_UNALIGNED_:       \
        default:                                    \
        {                                           \
            u8* _src = (u8*)(src);                  \
            u8* _dst = (u8*)(dst);                  \
            u8 _tmp[4];                             \
            _tmp[0] = _src[3];                      \
            _tmp[1] = _src[2];                      \
            _tmp[2] = _src[1];                      \
            _tmp[3] = _src[0];                      \
            _dst[0] = _tmp[0];                      \
            _dst[1] = _tmp[1];                      \
            _dst[2] = _tmp[2];                      \
            _dst[3] = _tmp[3];                      \
        }                                           \
        }                                           \
    }

#define swap_word64(src, dst, flag)             \
    {                                           \
        switch (flag) {                         \
        case 0:                                 \
        case SRC_UNALIGNED_:                    \
        case DST_UNALIGNED_:                    \
        case SRC_UNALIGNED_ | DST_UNALIGNED_:   \
        default:                                \
        {                                       \
            u8* _src = (u8*)(src);              \
            u8* _dst = (u8*)(dst);              \
            u8 _tmp[8];                         \
            _tmp[0] = _src[7];                  \
            _tmp[1] = _src[6];                  \
            _tmp[2] = _src[5];                  \
            _tmp[3] = _src[4];                  \
            _tmp[4] = _src[3];                  \
            _tmp[5] = _src[2];                  \
            _tmp[6] = _src[1];                  \
            _tmp[7] = _src[0];                  \
            _dst[0] = _tmp[0];                  \
            _dst[1] = _tmp[1];                  \
            _dst[2] = _tmp[2];                  \
            _dst[3] = _tmp[3];                  \
            _dst[4] = _tmp[4];                  \
            _dst[5] = _tmp[5];                  \
            _dst[6] = _tmp[6];                  \
            _dst[7] = _tmp[7];                  \
        }                                       \
        }                                       \
    }

#define swap_words32(src, dst, size, flag)              \
    {                                                   \
        switch (flag) {                                 \
        case 0:                                         \
        {                                               \
            u32* _src = (u32*)(src);                    \
            u32* _dst = (u32*)(dst);                    \
            size_t _size = (size_t)(size) >> 2;         \
            size_t _i;                                  \
                                                        \
            for (_i = 0; _i < _size; _i++) {            \
                _dst[_i] = (_src[_i] >> 24)             \
                        | ((_src[_i] & 0xff0000) >> 8)  \
                        | ((_src[_i] & 0xff00) << 8)    \
                        | (_src[_i] << 24);             \
            }                                           \
        }                                               \
        break;                                          \
        case SRC_UNALIGNED_:                            \
        case DST_UNALIGNED_:                            \
        case SRC_UNALIGNED_ | DST_UNALIGNED_:           \
        default:                                        \
        {                                               \
            u8* _src = (u8*)(src);                      \
            u8* _dst = (u8*)(dst);                      \
            size_t _size = (size_t)(size);              \
            size_t _i;                                  \
                                                        \
            for (_i = 0; _i < _size; _i += 4) {         \
                u8 _swap1 = _src[_i];                   \
                u8 _swap2 = _src[_i+1];                 \
                _dst[_i] =   _src[_i+3];                \
                _dst[_i+1] = _src[_i+2];                \
                _dst[_i+2] = _swap2;                    \
                _dst[_i+3] = _swap1;                    \
            }                                           \
        }                                               \
        }                                               \
    }

#define swap_words64(src, dst, size, flag)      \
    {                                           \
        u8* _src = (u8*)(src);                  \
        u8* _dst = (u8*)(dst);                  \
        u8 _next[4];                            \
        int _i, _j, _k;                         \
                                                \
        for (_i = 0; _i < (int)(size); ) {      \
            _j = _i;                            \
            _k = _i + 7;                        \
            _next[0] = _src[_j++];              \
            _next[1] = _src[_j++];              \
            _next[2] = _src[_j++];              \
            _next[3] = _src[_j];                \
            _dst[_i++] = _src[_k--];            \
            _dst[_i++] = _src[_k--];            \
            _dst[_i++] = _src[_k--];            \
            _dst[_i++] = _src[_k];              \
            _dst[_i++] = _next[3];              \
            _dst[_i++] = _next[2];              \
            _dst[_i++] = _next[1];              \
            _dst[_i++] = _next[0];              \
        }                                       \
    }

#if !defined(BYTE_ORDER) || !defined(LITTLE_ENDIAN) || !defined(BIG_ENDIAN)
#   error Byte order not defined!
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#   define cpu_to_le32_word(src, dst, flag) copy_word32(src, dst, flag)
#   define le32_to_cpu_word(src, dst, flag) copy_word32(src, dst, flag)
#   define cpu_to_be32_word(src, dst, flag) swap_word32(src, dst, flag)
#   define be32_to_cpu_word(src, dst, flag) swap_word32(src, dst, flag)
#   define cpu_to_le64_word(src, dst, flag) copy_word64(src, dst, flag)
#   define le64_to_cpu_word(src, dst, flag) copy_word64(src, dst, flag)
#   define cpu_to_be64_word(src, dst, flag) swap_word64(src, dst, flag)
#   define be64_to_cpu_word(src, dst, flag) swap_word64(src, dst, flag)
#   define cpu_to_le32(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#   define le32_to_cpu(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#   define cpu_to_be32(src, dst, size, flag) swap_words32(src, dst, size, flag)
#   define be32_to_cpu(src, dst, size, flag) swap_words32(src, dst, size, flag)
#   define cpu_to_le64(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#   define le64_to_cpu(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#   define cpu_to_be64(src, dst, size, flag) swap_words64(src, dst, size, flag)
#   define be64_to_cpu(src, dst, size, flag) swap_words64(src, dst, size, flag)
#elif BYTE_ORDER == BIG_ENDIAN
#   define cpu_to_le32_word(src, dst, flag) swap_word32(src, dst, flag)
#   define le32_to_cpu_word(src, dst, flag) swap_word32(src, dst, flag)
#   define cpu_to_be32_word(src, dst, flag) copy_word32(src, dst, flag)
#   define be32_to_cpu_word(src, dst, flag) copy_word32(src, dst, flag)
#   define cpu_to_le64_word(src, dst, flag) swap_word64(src, dst, flag)
#   define le64_to_cpu_word(src, dst, flag) swap_word64(src, dst, flag)
#   define cpu_to_be64_word(src, dst, flag) copy_word64(src, dst, flag)
#   define be64_to_cpu_word(src, dst, flag) copy_word64(src, dst, flag)
#   define cpu_to_le32(src, dst, size, flag) swap_words32(src, dst, size, flag)
#   define le32_to_cpu(src, dst, size, flag) swap_words32(src, dst, size, flag)
#   define cpu_to_be32(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#   define be32_to_cpu(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#   define cpu_to_le64(src, dst, size, flag) swap_words64(src, dst, size, flag)
#   define le64_to_cpu(src, dst, size, flag) swap_words64(src, dst, size, flag)
#   define cpu_to_be64(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#   define be64_to_cpu(src, dst, size, flag) if ((void *)(src) != (void *)(dst)) memmove(dst, src, size)
#else
#error BYTE_ORDER nor BIG_ nor LITTLE_ ENDIAN!
#endif

#if defined(WORD32)
#   define cpu_to_le_word(src, dst, flag) cpu_to_le32_word(src, dst, flag)
#   define le_to_cpu_word(src, dst, flag) le32_to_cpu_word(src, dst, flag)
#   define cpu_to_be_word(src, dst, flag) cpu_to_be32_word(src, dst, flag)
#   define be_to_cpu_word(src, dst, flag) be32_to_cpu_word(src, dst, flag)
#   define cpu_to_le(src, dst, size, flag) cpu_to_le32(src, dst, size, flag)
#   define le_to_cpu(src, dst, size, flag) le32_to_cpu(src, dst, size, flag)
#   define cpu_to_be(src, dst, size, flag) cpu_to_be32(src, dst, size, flag)
#   define be_to_cpu(src, dst, size, flag) be32_to_cpu(src, dst, size, flag)
#elif defined(WORD64)
#   define cpu_to_le_word(src, dst, flag) cpu_to_le64_word(src, dst, flag)
#   define le_to_cpu_word(src, dst, flag) le64_to_cpu_word(src, dst, flag)
#   define cpu_to_be_word(src, dst, flag) cpu_to_be64_word(src, dst, flag)
#   define be_to_cpu_word(src, dst, flag) be64_to_cpu_word(src, dst, flag)
#   define cpu_to_le(src, dst, size, flag) cpu_to_le64(src, dst, size, flag)
#   define le_to_cpu(src, dst, size, flag) le64_to_cpu(src, dst, size, flag)
#   define cpu_to_be(src, dst, size, flag) cpu_to_be64(src, dst, size, flag)
#   define be_to_cpu(src, dst, size, flag) be64_to_cpu(src, dst, size, flag)
#else
#   error Word size not defined!
#endif

#endif  // __BYTESWAP_HPP__
