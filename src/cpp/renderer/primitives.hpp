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

#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include "renderer.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "vector.hpp"
#include "geometry.hpp"

namespace renderer
{

class PRIMITIVES
{
public:
    static void draw_background(const TEXTURE& image, float attenuation = 1);
    static void draw_normal(const Math::P3D& position, const Math::V3D& direction, const float length);
    static void draw_aim(const camera::MCAMERA& camera, const TEXTURE& crest);
    static void draw_skybox(const camera::MCAMERA& camera, const TEXTURE& up, const TEXTURE& dn, const TEXTURE& ft, const TEXTURE& bk, const TEXTURE& lf, const TEXTURE& rt);
    static void draw_sprite(const Math::P3D& position, const camera::MCAMERA& camera, const TEXTURE& img, const Math::P3D& color, float scale, float angle = 0);
    static void draw_cube(const Math::P3D& centre, float half_edge);
    static void draw_cube(const Math::P3D& centre, float half_edge, const Math::P3D& color);
    static void draw_box(const Math::P3D& centre, const Math::P3D& size_xyz);
    static void draw_line(const Math::P3D& b, const Math::P3D& e, const Math::P3D& b_color, const Math::P3D& e_color);
    static void draw_line_strip(const Math::P3D* pnts, int pnts_cnt, const Math::P3D& color);
    static void draw_wire_sphere(const Math::P3D& centre, float radius, const Math::P3D& color);
    static void draw_triangle(const Math::Triangle& triangle, const Math::P3D& color);
    static void draw_solid_sphere(const Math::P3D& centre, float radius, const Math::P3D& color);
};

} // namespace renderer

#endif // __PRIMITIVES_HPP__
