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

#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "renderer.hpp"
#include "vector.hpp"

namespace renderer
{

class LIGHT
{
private:
    static GLenum* srcs;
    static bool* actv;
    static int srcs_cnt;

    int idx;

    int get_source();
    void free_source(int);

    void apply_all();
    void apply_direction();

private:
    float ambient[4];                       // ambient RGBA intensity
    float diffuse[4];                       // diffuse RGBA intensity
    float specular[4];                      // specular RGBA intensity

    float position[4];                      // position in homogeneous object coordinates, if w != 0 then not directional
    float direction[3];                     // direction in homogeneous object coordinates

    float const_att[0];                     // attenuation factor
    float line_att[0];                      // attenuation factor
    float quad_att[0];                      // attenuation factor
    float exponent[0];                      // intensity distribution [0, 128]
    float cutoff[0];                        // maximum spread angle [0, 90]

public:
    LIGHT();
    ~LIGHT();

    LIGHT(const LIGHT&);
    LIGHT& operator =(const LIGHT&);

    void apply(const math::P3D& ambient, const math::P3D& diffuse, const math::P3D& specular,
            const math::V3D& direction);
    void apply(const math::P3D& ambient, const math::P3D& diffuse, const math::P3D& specular,
            const math::P3D& position, float const_att, float line_att, float quad_att);
    void apply(const math::P3D& ambient, const math::P3D& diffuse, const math::P3D& specular,
            const math::P3D& position, const math::V3D& direction, float const_att, float line_att, float quad_att, float exponent, float cutoff);
    void apply(const math::P3D& ambient, const math::P3D& diffuse, const math::P3D& specular,
            const math::P3D& origin, const math::P3D& target, float const_att, float line_att, float quad_att, float exponent, float cutoff);
    void apply(const math::P3D& origin, const math::P3D& target);

    void switch_on() const;
    void switch_off() const;

    static void enable();
    static void disable();

    static int get_max_sources();
    static int get_sources_left();
};

} // namespace renderer

#endif  // __LIGHT_HPP__
