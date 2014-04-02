#include <math.h>
#include "renderer.hpp"
#include "vector.hpp"
#include "camera.hpp"

using namespace math;
using namespace renderer;

const P3D NP(0, 0, 0);
const V3D Vi(1, 0, 0);
const V3D Vj(0, 1, 0);
const V3D Vk(0, 0, 1);

const float PI = 3.1415926f;

CAMERA::CAMERA()
{
    position = NP;
    i = Vi;
    j = Vj;
    k = Vk;
    yaw = 0;
}

CAMERA::CAMERA(const P3D& p)
{
    position = p;
    i = Vi;
    j = Vj;
    k = Vk;
    yaw = 0;
}

CAMERA::CAMERA(const P3D& p, const V3D& v)
{
    position = p;
    i = Vi;
    j = v;
    j.set_length(1);
    k = Vk;
    yaw = 0;
}

void CAMERA::set_camera(const P3D& p, const P3D& t)
{
    position = p;
    V3D v(p, t);
    v.set_length(1);

    j = v;

    v = j * Vk;
    k = v * j;

    i = j * k;
}

void CAMERA::set_vectors(const V3D& x, const V3D& y, const V3D& z)
{
    i = x;
    j = y;
    k = z;
}

void CAMERA::set_angles(const float& h, const float& v, const float& b)
{
    pitch = h;
    roll = v;
    yaw = b;
    roll *= PI / 180;
    pitch *= PI / 180;
    yaw *= PI / 180;

    i = Vi;
    j = Vj;
    k = Vk;
    V3D c, d, t;

    /* rotating around Z-axis */
    c = i;
    d = j;
    c.mult_by(-sin(pitch));
    d.mult_by(cos(pitch));
    i.mult_by(cos(pitch));
    j.mult_by(sin(pitch));
    i = i + j;
    i.set_length(1);
    j = c + d;
    j.set_length(1);

    /* rotating around X-axis */
    c = j;
    d = k;
    c.mult_by(-cos(roll));
    d.mult_by(sin(roll));
    j.mult_by(sin(roll));
    k.mult_by(cos(roll));
    j = j + k;
    j.set_length(1);
    k = c + d;
    k.set_length(1);
    t = i;

    /* rotating around Y-axis */
    c = i;
    d = k;
    c.mult_by(-sin(yaw));
    d.mult_by(cos(yaw));
    i.mult_by(cos(yaw));
    k.mult_by(sin(yaw));
    i = i + k;
    i.set_length(1);
    k = c + d;
    k.set_length(1);
}

void CAMERA::apply()
{
    glLoadIdentity();
    glRotated(yaw, 0, 0, 1);
    j.set_length(1);
    gluLookAt(position.x, position.y, position.z,
            position.x + j.dir.x,
            position.y + j.dir.y,
            position.z + j.dir.z,
            0, 0, 1);
}

void CAMERA::apply(const P3D& p, const P3D& t)
{
    set_camera(p, t);
    apply();
}

void CAMERA::apply(const V3D& X, const V3D& Y, const V3D& Z)
{
    set_vectors(X, Y, Z);
    apply();
}

void CAMERA::apply(const float& h, const float& v, const float& b)
{
    set_angles(h, v, b);
    apply();
}
