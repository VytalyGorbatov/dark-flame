#include <iostream>
#include "utils.hpp"

extern void unit_vector(void);

int main()
{
    unit_vector(void);

    std::cout << "Total errors: " << error_count << std::endl << std::endl;

    return 0;
}
