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
