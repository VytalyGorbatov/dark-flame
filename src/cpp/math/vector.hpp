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

#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

namespace math
{

/** Math point in Cartesian coordinate system. */
class P3D
{
public:
    float x, y, z;

public:
    P3D()
    {
        x = 0; y = 0; z = 0;
    }

    P3D(const float& a, const float& b, const float& c)
    {
        x = a; y = b; z = c;
    }

    P3D& mult_by(const float& a)
    {
        x *= a; y *= a; z *= a;
        return *this;
    }

    P3D& devide_by(const float& a)
    {
        x /= a; y /= a; z /= a;
        return *this;
    }

    void set_xyz(const float& a, const float& b, const float& c)
    {
        x = a; y = b; z = c;
    }

    float get_distance(const P3D& point) const;
};

inline P3D operator +(P3D lh, const P3D& rh)
{
    lh.x += rh.x;
    lh.y += rh.y;
    lh.z += rh.z;
    return lh;
}

inline P3D operator -(P3D lh, const P3D& rh)
{
    lh.x -= rh.x;
    lh.y -= rh.y;
    lh.z -= rh.z;
    return lh;
}

/** Vector in Cartesian coordinate system. */
class V3D
{
public:
    P3D dir;

public:
    V3D()
    {
        dir.x = 0;
        dir.y = 0;
        dir.z = 0;
    }

    V3D(const P3D& v)
    {
        dir = v;
    }

    V3D(const P3D& b, const P3D& e)
    {
        dir.x = e.x - b.x;
        dir.y = e.y - b.y;
        dir.z = e.z - b.z;
    }

    V3D(const float& i, const float& j, const float& k)
    {
        dir.x = i;
        dir.y = j;
        dir.z = k;
    }

    V3D(const V3D& v)
    {
        dir = v.dir;
    }

    V3D operator -()
    {
        V3D res;
        res.dir.x = -dir.x;
        res.dir.y = -dir.y;
        res.dir.z = -dir.z;
        return res;
    }

    V3D operator *(const V3D& v)
    {
        V3D res;
        res.dir.x = dir.y * v.dir.z - dir.z * v.dir.y;
        res.dir.y = - dir.x * v.dir.z + dir.z * v.dir.x;
        res.dir.z = dir.x * v.dir.y - dir.y * v.dir.x;
        return res;
    }

    V3D& mult_by(const float& a)
    {
        dir.x *= a;
        dir.y *= a;
        dir.z *= a;
        return *this;
    }

    float abs_mult(const V3D& v) const
    {
        return dir.x * v.dir.x + dir.y * v.dir.y + dir.z * v.dir.z;
    }

    float abs_mult(const P3D& v) const
    {
        return dir.x * v.x + dir.y * v.y + dir.z * v.z;
    }

    float full_mult(const V3D& v, const V3D& w) const
    {
        float res;
        res = dir.x * (v.dir.y * w.dir.z - v.dir.z * w.dir.y);
        res -= dir.y * (v.dir.x * w.dir.z - v.dir.z * w.dir.x);
        res += dir.z * (v.dir.x * w.dir.y - v.dir.y * w.dir.x);
        return res;
    }

    P3D add_to(const P3D& p) const
    {
        P3D result;
        result.x = p.x + dir.x;
        result.y = p.y + dir.y;
        result.z = p.z + dir.z;
        return result;
    }

    float get_length() const;
    float get_distance(const P3D& point) const;
    bool set_length(const float& length);
    bool projection(const V3D& base);
};

inline V3D operator +(V3D lh, const V3D& rh)
{
    lh.dir.x += rh.dir.x;
    lh.dir.y += rh.dir.y;
    lh.dir.z += rh.dir.z;
    return lh;
}

inline V3D operator -(V3D lh, const V3D& rh)
{
    lh.dir.x -= rh.dir.x;
    lh.dir.y -= rh.dir.y;
    lh.dir.z -= rh.dir.z;
    return lh;
}

} // namespace math

#endif // __VECTOR_HPP__
