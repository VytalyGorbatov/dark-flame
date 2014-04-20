#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include "vector.hpp"

namespace math
{

class GEOMETRY
{
private:
    float piramid_vis[6][4];

public:
    /**
     * Checks if trianlge cross the cube(hitbox).
     * Edges are oriented by normal vectors i, j ,k.
     *
     * @return true - some part of triangle lies inside hitbox
     */
    static bool is_triangle_in_cube(const P3D& centre, float half_edge, const P3D& a, const P3D& b, const P3D& c);

    /**
     * Checks if line segment locate inside cube(hitbox).
     * Edges are oriented by normal vectors i, j ,k.
     *
     * Sphere as a hitbox is using.
     *
     * @param false - whole line lies outside cube
     */
    static bool is_line_in_cube(const P3D& centre, float half_edge, const P3D& begin, const P3D& end);

    /**
     * Checks if some part of the sphere(hitbox) lies inside piramid of vision.
     *
     * @return distance to the near clipping plane, zero if invisible
     */
    float is_sphere_visible(const P3D& centre, float radius) const;

    /**
     * Cheks if cube lies inside piramid of vision.
     *
     * @return true - some part of cube is visible
     */
    bool is_cube_visible(const P3D& centre, float half_edge) const;

    /**
     * Cheks if cube lies inside piramid of vision.
     *
     * @return 0 - box is invisible, 1 - partialy visible, 2 - whole cube is visible
     */
    int is_box_visible(const P3D& origin, const P3D& sides) const;

    /**
     * Calculates reflection matrix for a given plane.
     *
     * @param reflection_matrix pointer to 'float[4][4]' array
     */
    static void calc_mirror_matrix(const P3D& plane_point, const V3D& plane_normal, float* reflection_matrix);

    /**
     * Calculates planes for piramid of vision.
     *
     * @param proj pointer to projection view matrix 'float[4][4]'
     * @param modl pointer to model view matrix 'float[4][4]'
     */
    void set_piramid_vis(const float* proj, const float* modl);
};

} // namespace math

#endif // __GEOMETRY_HPP__
