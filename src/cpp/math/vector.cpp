/**
 * DarkFlame Copyright (C) 2022 Alexey Shumeiko
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
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif
#include "vector.hpp"
#include <math.h>

using namespace Math;

float P3D::get_distance(const P3D& p) const
{
    return static_cast<float>(sqrt((p.x - x) * (p.x - x)
                + (p.y - y) * (p.y - y)
                + (p.z - z) * (p.z - z)));
}

bool V3D::set_length(const float& nl)
{
    float l = static_cast<float>(sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
    if (l < 0.000001)
        return false;
    register float k = nl / l;
    dir.x = dir.x * k;
    dir.y = dir.y * k;
    dir.z = dir.z * k;
    return true;
}

bool V3D::projection(const V3D& v)
{
    float vls = v.dir.x * v.dir.x + v.dir.y * v.dir.y + v.dir.z * v.dir.z;
    if (vls < 0.000001f)
        return false;
    register float k = (dir.x * v.dir.x + dir.y * v.dir.y + dir.z * v.dir.z) / vls;
    dir.x = v.dir.x * k;
    dir.y = v.dir.y * k;
    dir.z = v.dir.z * k;
    return true;
}

float V3D::get_distance(const P3D& p) const
{
    return static_cast<float>(sqrt((p.x - dir.x) * (p.x - dir.x)
                + (p.y - dir.y) * (p.y - dir.y)
                + (p.z - dir.z) * (p.z - dir.z)));
}

float V3D::get_length() const
{
    return static_cast<float>(sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
}
