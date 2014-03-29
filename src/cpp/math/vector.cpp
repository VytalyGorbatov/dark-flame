#include <math.h>
#include "vector.hpp"

float P3D::GetDistance(const P3D& p) const
{
    return (float)sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z));
}

void P3D::MultBy(const float& a)
{
    x *= a; y *= a; z *= a;
}

void P3D::SetXYZ(const float& a, const float& b, const float& c)
{
    x = a; y = b; z = c;
}

V3D::V3D()
{
    dir.x = 0;
    dir.y = 0;
    dir.z = 0;
}

V3D::V3D(const P3D& v)
{
    dir = v;
}

V3D::V3D(const P3D& b, const P3D& e)
{
    dir.x = e.x - b.x;
    dir.y = e.y - b.y;
    dir.z = e.z - b.z;
}

V3D::V3D(const float& x, const float& y, const float& z)
{
    dir.x = x;
    dir.y = y;
    dir.z = z;
}

V3D::V3D(const V3D& v)
{
    dir = v.dir;
}

bool V3D::SetLength(const float& nl)
{
    float l = (float)sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (l < 0.000001) return false;
    register float k = nl / l;
    dir.x = dir.x * k;
    dir.y = dir.y * k;
    dir.z = dir.z * k;
    return true;
}

bool V3D::Projection(const V3D& v)
{
    float vls = v.dir.x * v.dir.x + v.dir.y * v.dir.y + v.dir.z * v.dir.z;
    if (vls < 0.000001f) return false;
    register float k = (dir.x * v.dir.x + dir.y * v.dir.y + dir.z * v.dir.z) / vls;
    dir.x = v.dir.x * k;
    dir.y = v.dir.y * k;
    dir.z = v.dir.z * k;
    return true;
}

float V3D::GetDistance(const P3D& p) const
{
    return (float)sqrt((p.x - dir.x) * (p.x - dir.x)
                     + (p.y - dir.y) * (p.y - dir.y)
                     + (p.z - dir.z) * (p.z - dir.z));
}

float V3D::GetLength() const
{
    return (float)sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
}