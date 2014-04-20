#include "geometry.hpp"
#include "vector.hpp"

using namespace math;

bool is_triangle_in_cube(const P3D& centre, float half_edge, const P3D& a, const P3D& b, const P3D& c)
{
    float ck;

    /* coordinate X */
    ck = centre.x - half_edge;
    if (a.x < ck && b.x < ck && c.x < ck) {
        return false;
    }
    ck = centre.x + half_edge;
    if (a.x > ck && b.x > ck && c.x > ck) {
        return false;
    }

    /* coordinate Y */
    ck = centre.y - half_edge;
    if (a.y < ck && b.y < ck && c.y < ck) {
        return false;
    }
    ck = centre.y + half_edge;
    if (a.y > ck && b.y > ck && c.y > ck) {
        return false;
    }

    /* coordinate Z */
    ck = centre.z - half_edge;
    if (a.z < ck && b.z < ck && c.z < ck) {
        return false;
    }
    ck = centre.z + half_edge;
    if (a.z > ck && b.z > ck && c.z > ck) {
        return false;
    }

    return true;
}
