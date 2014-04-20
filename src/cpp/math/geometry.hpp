#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include "vector.hpp"

namespace math
{

bool is_triangle_in_cube(const P3D& centre, float half_edge, const P3D& a, const P3D& b, const P3D& c);

} // namespace math

#endif // __GEOMETRY_HPP__
