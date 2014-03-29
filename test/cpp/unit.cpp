#include <iostream>
#include "utils.hpp"

extern void unit_vector(void);
extern void unit_particles(void);

int main()
{
    unit_vector();
    unit_particles();

    std::cout << "Total errors: " << error_count << std::endl << std::endl;

    return 0;
}
