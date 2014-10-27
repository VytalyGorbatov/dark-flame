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

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <string.h>

extern int error_count;

#define ASSERT_NULL(ptr)                                                \
    {                                                                   \
        if (!(ptr)) {                                                   \
            std::cout << __FILE__ << ":" << __LINE__ << ": ";           \
            std::cout << "assert failed (null ptr)" << std::endl;       \
            error_count++;                                              \
        }                                                               \
    }

#define ASSERT_TRUE(expr)                                               \
    {                                                                   \
        if (!(expr)) {                                                  \
            std::cout << __FILE__ << ":" << __LINE__ << ": ";           \
            std::cout << "assert failed (false)" << std::endl;          \
            error_count++;                                              \
        }                                                               \
    }

#define ASSERT_EQUALS(exp, act)                                         \
    {                                                                   \
        if (memcmp(exp, act, sizeof(*(exp)))) {                         \
            std::cout << __FILE__ << ":" << __LINE__ << ": ";           \
            std::cout << "assert failed (not equals)" << std::endl;     \
            error_count++;                                              \
        }                                                               \
    }

#endif // __UTILS_HPP__
