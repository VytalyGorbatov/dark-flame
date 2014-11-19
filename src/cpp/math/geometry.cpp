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

#include <math.h>
#include "geometry.hpp"
#include "vector.hpp"

using namespace math;

/**
 * Checks if trianlge cross the cube(hitbox).
 * Edges are oriented by normal vectors i, j ,k.
 *
 * @return true - some part of triangle lies inside hitbox
 */
bool GEOMETRY::is_triangle_in_cube(const P3D& centre, float half_edge, const P3D& a, const P3D& b, const P3D& c)
{
    float ck;

    /* coordinate X */
    ck = centre.x - half_edge;
    if (a.x < ck && b.x < ck && c.x < ck) return false;
    ck = centre.x + half_edge;
    if (a.x > ck && b.x > ck && c.x > ck) return false;

    /* coordinate Y */
    ck = centre.y - half_edge;
    if (a.y < ck && b.y < ck && c.y < ck) return false;
    ck = centre.y + half_edge;
    if (a.y > ck && b.y > ck && c.y > ck) return false;

    /* coordinate Z */
    ck = centre.z - half_edge;
    if (a.z < ck && b.z < ck && c.z < ck) return false;
    ck = centre.z + half_edge;
    if (a.z > ck && b.z > ck && c.z > ck) return false;

    return true;
}

/**
 * Checks if line segment locate inside cube(hitbox).
 * Edges are oriented by normal vectors i, j ,k.
 *
 * Sphere as a hitbox is using.
 *
 * @param false - whole line lies outside cube
 */
bool GEOMETRY::is_line_in_cube(const P3D& centre, float half_edge, const P3D& begin, const P3D& end)
{
    float r = half_edge * 1.41421356f;
    float d;

    V3D line(begin, end);
    V3D r1(begin, centre);
    r1.projection(line);

    P3D A = r1.dir + begin;

    d = centre.get_distance(A);
    if (d > r) return false;

    d = centre.get_distance(begin);
    if (d <= r) return true;

    d = centre.get_distance(end);
    if (d <= r) return true;

    V3D r2 = line + r1;
    float a, b, c;

    a = r1.get_length();
    b = line.get_length();
    c = r2.get_length();
    if (a <= b && c >= a + b - 0.000001f) return true;

    return false;
}

/**
 * Checks if some part of the sphere(hitbox) lies inside piramid of vision.
 *
 * @return distance to the near clipping plane, zero if invisible
 */
float GEOMETRY::is_sphere_visible(const P3D& centre, float radius) const
{
    float d;

    d = piramid_vis[0][0] * centre.x + piramid_vis[0][1] * centre.y + piramid_vis[0][2] * centre.z + piramid_vis[0][3];
    if (d <= -radius) return 0;

    d = piramid_vis[1][0] * centre.x + piramid_vis[1][1] * centre.y + piramid_vis[1][2] * centre.z + piramid_vis[1][3];
    if (d <= -radius) return 0;

    d = piramid_vis[2][0] * centre.x + piramid_vis[2][1] * centre.y + piramid_vis[2][2] * centre.z + piramid_vis[2][3];
    if (d <= -radius) return 0;

    d = piramid_vis[3][0] * centre.x + piramid_vis[3][1] * centre.y + piramid_vis[3][2] * centre.z + piramid_vis[3][3];
    if (d <= -radius) return 0;

    d = piramid_vis[4][0] * centre.x + piramid_vis[4][1] * centre.y + piramid_vis[4][2] * centre.z + piramid_vis[4][3];
    if (d <= -radius) return 0;

    d = piramid_vis[5][0] * centre.x + piramid_vis[5][1] * centre.y + piramid_vis[5][2] * centre.z + piramid_vis[5][3];
    if (d <= -radius) return 0;

    return d + radius;
}

/**
 * Cheks if cube lies inside piramid of vision.
 *
 * @return true - some part of cube is visible
 */
bool GEOMETRY::is_cube_visible(const P3D& centre, float half_edge) const
{
    float xp ,xm, yp, ym, zp, zm;

    for (int i = 0; i < 6; ++i) {
        xm = piramid_vis[i][0] * (centre.x - half_edge);
        ym = piramid_vis[i][1] * (centre.y - half_edge);
        zm = piramid_vis[i][2] * (centre.z - half_edge);
        if (xm + ym + zm + piramid_vis[i][3] > 0) continue;

        xp = piramid_vis[i][0] * (centre.x + half_edge);
        if (xp + ym + zm + piramid_vis[i][3] > 0) continue;

        yp = piramid_vis[i][1] * (centre.y + half_edge);
        if (xm + yp + zm + piramid_vis[i][3] > 0) continue;
        if (xp + yp + zm + piramid_vis[i][3] > 0) continue;

        zp = piramid_vis[i][2] * (centre.z + half_edge);
        if (xm + ym + zp + piramid_vis[i][3] > 0) continue;
        if (xp + ym + zp + piramid_vis[i][3] > 0) continue;
        if (xm + yp + zp + piramid_vis[i][3] > 0) continue;
        if (xp + yp + zp + piramid_vis[i][3] > 0) continue;

        return false;
    }

    return true;
}

/**
 * Cheks if cube lies inside piramid of vision.
 *
 * @return 0 - box is invisible, 1 - partialy visible, 2 - whole cube is visible
 */
int GEOMETRY::is_box_visible(const P3D& origin, const P3D& sides) const
{
    float xp ,xm, yp, ym, zp, zm;
    int p = 0;

    for (int i = 0; i < 6; ++i) {
        int c = 0;

        xp = piramid_vis[i][0] * (origin.x + sides.x);
        xm = piramid_vis[i][0] * origin.x;
        yp = piramid_vis[i][1] * (origin.y + sides.y);
        ym = piramid_vis[i][1] * origin.y;
        zp = piramid_vis[i][2] * (origin.z + sides.z);
        zm = piramid_vis[i][2] * origin.z;

        if (xm + ym + zm + piramid_vis[i][3] > 0) c++;
        if (xp + ym + zm + piramid_vis[i][3] > 0) c++;
        if (xm + yp + zm + piramid_vis[i][3] > 0) c++;
        if (xp + yp + zm + piramid_vis[i][3] > 0) c++;
        if (xm + ym + zp + piramid_vis[i][3] > 0) c++;
        if (xp + ym + zp + piramid_vis[i][3] > 0) c++;
        if (xm + yp + zp + piramid_vis[i][3] > 0) c++;
        if (xp + yp + zp + piramid_vis[i][3] > 0) c++;

        if (c == 0) {
            return 0;
        }

        if (c == 8) {
            p++;
        }
    }

    return (p == 6) ? 2 : 1;
}

/**
 * Calculates reflection matrix for a given plane.
 *
 * @param reflection_matrix pointer to 'float[4][4]' array
 */
void GEOMETRY::calc_mirror_matrix(const P3D& plane_point, const V3D& plane_normal, float* reflection_matrix)
{
    if (!reflection_matrix) {
        return;
    }

    float d = plane_point.x * plane_normal.dir.x + plane_point.y * plane_normal.dir.y + plane_point.z * plane_normal.dir.z;

    reflection_matrix[0 * 4 + 0] = 1 - 2 * plane_normal.dir.x * plane_normal.dir.x;
    reflection_matrix[1 * 4 + 0] =   - 2 * plane_normal.dir.x * plane_normal.dir.y;
    reflection_matrix[2 * 4 + 0] =   - 2 * plane_normal.dir.x * plane_normal.dir.z;
    reflection_matrix[3 * 4 + 0] = 2 * d * plane_normal.dir.x;

    reflection_matrix[0 * 4 + 1] =   - 2 * plane_normal.dir.y * plane_normal.dir.x;
    reflection_matrix[1 * 4 + 1] = 1 - 2 * plane_normal.dir.y * plane_normal.dir.y;
    reflection_matrix[2 * 4 + 1] =   - 2 * plane_normal.dir.y * plane_normal.dir.z;
    reflection_matrix[3 * 4 + 1] = 2 * d * plane_normal.dir.y;

    reflection_matrix[0 * 4 + 2] =   - 2 * plane_normal.dir.z * plane_normal.dir.x;
    reflection_matrix[1 * 4 + 2] =   - 2 * plane_normal.dir.z * plane_normal.dir.y;
    reflection_matrix[2 * 4 + 2] = 1 - 2 * plane_normal.dir.z * plane_normal.dir.z;
    reflection_matrix[3 * 4 + 2] = 2 * d * plane_normal.dir.z;

    reflection_matrix[0 * 4 + 3] = 0;
    reflection_matrix[1 * 4 + 3] = 0;
    reflection_matrix[2 * 4 + 3] = 0;
    reflection_matrix[3 * 4 + 3] = 1;
}

/**
 * Calculates planes for piramid of vision.
 *
 * @param proj pointer to projection view matrix 'float[16]'
 * @param modl pointer to model view matrix 'float[16]'
 */
void GEOMETRY::set_piramid_vis(const float* proj, const float* modl)
{
    float clip[16];
    float t;

    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

    /* right */
    piramid_vis[0][0] = clip[ 3] - clip[ 0];
    piramid_vis[0][1] = clip[ 7] - clip[ 4];
    piramid_vis[0][2] = clip[11] - clip[ 8];
    piramid_vis[0][3] = clip[15] - clip[12];
    t = sqrt(piramid_vis[0][0] * piramid_vis[0][0] + piramid_vis[0][1] * piramid_vis[0][1] + piramid_vis[0][2] * piramid_vis[0][2]);
    piramid_vis[0][0] /= t;
    piramid_vis[0][1] /= t;
    piramid_vis[0][2] /= t;
    piramid_vis[0][3] /= t;

    /* left */
    piramid_vis[1][0] = clip[ 3] + clip[ 0];
    piramid_vis[1][1] = clip[ 7] + clip[ 4];
    piramid_vis[1][2] = clip[11] + clip[ 8];
    piramid_vis[1][3] = clip[15] + clip[12];
    t = sqrt(piramid_vis[1][0] * piramid_vis[1][0] + piramid_vis[1][1] * piramid_vis[1][1] + piramid_vis[1][2] * piramid_vis[1][2]);
    piramid_vis[1][0] /= t;
    piramid_vis[1][1] /= t;
    piramid_vis[1][2] /= t;
    piramid_vis[1][3] /= t;

    /* bottom */
    piramid_vis[2][0] = clip[ 3] + clip[ 1];
    piramid_vis[2][1] = clip[ 7] + clip[ 5];
    piramid_vis[2][2] = clip[11] + clip[ 9];
    piramid_vis[2][3] = clip[15] + clip[13];
    t = sqrt(piramid_vis[2][0] * piramid_vis[2][0] + piramid_vis[2][1] * piramid_vis[2][1] + piramid_vis[2][2] * piramid_vis[2][2]);
    piramid_vis[2][0] /= t;
    piramid_vis[2][1] /= t;
    piramid_vis[2][2] /= t;
    piramid_vis[2][3] /= t;

    /* top */
    piramid_vis[3][0] = clip[ 3] - clip[ 1];
    piramid_vis[3][1] = clip[ 7] - clip[ 5];
    piramid_vis[3][2] = clip[11] - clip[ 9];
    piramid_vis[3][3] = clip[15] - clip[13];
    t = sqrt(piramid_vis[3][0] * piramid_vis[3][0] + piramid_vis[3][1] * piramid_vis[3][1] + piramid_vis[3][2] * piramid_vis[3][2]);
    piramid_vis[3][0] /= t;
    piramid_vis[3][1] /= t;
    piramid_vis[3][2] /= t;
    piramid_vis[3][3] /= t;

    /* back */
    piramid_vis[4][0] = clip[ 3] - clip[ 2];
    piramid_vis[4][1] = clip[ 7] - clip[ 6];
    piramid_vis[4][2] = clip[11] - clip[10];
    piramid_vis[4][3] = clip[15] - clip[14];
    t = sqrt(piramid_vis[4][0] * piramid_vis[4][0] + piramid_vis[4][1] * piramid_vis[4][1] + piramid_vis[4][2] * piramid_vis[4][2]);
    piramid_vis[4][0] /= t;
    piramid_vis[4][1] /= t;
    piramid_vis[4][2] /= t;
    piramid_vis[4][3] /= t;

    /* front */
    piramid_vis[5][0] = clip[ 3] + clip[ 2];
    piramid_vis[5][1] = clip[ 7] + clip[ 6];
    piramid_vis[5][2] = clip[11] + clip[10];
    piramid_vis[5][3] = clip[15] + clip[14];
    t = sqrt(piramid_vis[5][0] * piramid_vis[5][0] + piramid_vis[5][1] * piramid_vis[5][1] + piramid_vis[5][2] * piramid_vis[5][2]);
    piramid_vis[5][0] /= t;
    piramid_vis[5][1] /= t;
    piramid_vis[5][2] /= t;
    piramid_vis[5][3] /= t;
}

TRIANGLE::TRIANGLE(const P3D& a, const P3D& b, const P3D& c) : A(a), B(b), C(c)
{
    V3D v1(A, B);
    V3D v2(A, C);

    normal = v1 * v2;

    if (0 == normal.get_length()) {
        normal.dir.set_xyz(0, 0, 0);
        d = 0;
        is_deg = true;
    } else {
        normal.set_length(1.0f);
        d = normal.abs_mult(A);
        is_deg = false;
    }
}

bool TRIANGLE::is_degenerate() const
{
    return is_deg;
}

V3D TRIANGLE::get_normal() const
{
    return normal;
}

P3D TRIANGLE::get_collision(const P3D& b, const P3D& e) const
{
    if (is_deg) {
        return e;
    }

    float d1 = b.x * normal.dir.x + b.y * normal.dir.y + b.z * normal.dir.z - d;
    float d2 = e.x * normal.dir.x + e.y * normal.dir.y + e.z * normal.dir.z - d;

    if (d1 * d2 >= 0) {
        return e;
    }

    V3D r(b, e);
    r.set_length(1.0f);
    float t = (d - normal.abs_mult(b)) / r.abs_mult(normal);
    r.set_length(t);
    P3D result = r.add_to(b);

    // The method is based on determination of the relation of projections of vectors to a normal
    // V3D cv(b, e);
    // V3D ca(b, A);
    // float k = ca.abs_mult(normal) / cv.abs_mult(normal);
    // if (k < 0 || k > 1) {
    //     return e;
    // }
    // cv.mult_by(k);
    // P3D result = cv.add_to(b);

    V3D i(result, A);
    V3D j(result, B);
    V3D k(result, C);
    i.set_length(1.0f);
    j.set_length(1.0f);
    k.set_length(1.0f);

    float sum = acos(i.abs_mult(j)) + acos(j.abs_mult(k)) + acos(k.abs_mult(i));
    if (sum < 6.2829f) {
        return e;
    }

    return result;
}

P3D TRIANGLE::get_collision(const P3D& p, const V3D& v) const
{
    if (is_deg) {
        return p;
    }

    V3D r(v);
    r.set_length(1);
    V3D rs = r;
    float t = (d - normal.abs_mult(p)) / r.abs_mult(normal);
    r.set_length(t);
    P3D result = r.add_to(p);

    V3D i(result, A);
    V3D j(result, B);
    V3D k(result, C);
    i.set_length(1);
    j.set_length(1);
    k.set_length(1);

    float sum = acos(i.abs_mult(j)) + acos(j.abs_mult(k)) + acos(k.abs_mult(i));
    if (sum < 6.2829f) {
        t = (d - normal.abs_mult(p)) / rs.abs_mult(normal);
        rs.set_length(t);
        result = rs.add_to(p);
    }

    return result;
}
