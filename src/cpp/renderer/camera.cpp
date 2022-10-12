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
#include "ogl.hpp"
#include "renderer.hpp"
#include "vector.hpp"
#include "camera.hpp"

using namespace Math;
using namespace renderer;
using namespace renderer::camera;

const P3D NP(0, 0, 0);
const V3D Vi(1, 0, 0);
const V3D Vj(0, 1, 0);
const V3D Vk(0, 0, 1);

const float PI = 3.1415926f;

MCAMERA::MCAMERA()
{
    position = NP;
    i = Vi;
    j = Vj;
    k = Vk;
    yaw = 0;
}

MCAMERA::MCAMERA(const P3D& p)
{
    position = p;
    i = Vi;
    j = Vj;
    k = Vk;
    yaw = 0;
}

MCAMERA::MCAMERA(const P3D& p, const V3D& v)
{
    position = p;
    i = Vi;
    j = v;
    j.set_length(1);
    k = Vk;
    yaw = 0;
}

MCAMERA::MCAMERA(const P3D& p, const P3D& t)
{
    position = p;
    V3D v(p, t);
    v.set_length(1);

    j = v;
    v = j * Vk;
    k = v * j;
    i = j * k;

    yaw = 0;
}

void MCAMERA::set_camera(const P3D& p, const P3D& t)
{
    position = p;
    V3D v(p, t);
    v.set_length(1);

    j = v;

    v = j * Vk;
    k = v * j;

    i = j * k;
}

void MCAMERA::set_camera(const P3D& p, const V3D& v)
{
    position = p;
    i = Vi;
    j = v;
    j.set_length(1);
    k = Vk;
    yaw = 0;
}

void MCAMERA::set_vectors(const V3D& x, const V3D& y, const V3D& z)
{
    i = x;
    j = y;
    k = z;
}

void MCAMERA::set_angles(const float h, const float v, const float b)
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

void MCAMERA::apply()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(yaw, 0, 0, 1);
    j.set_length(1);
    gluLookAt(position.x, position.y, position.z,
            position.x + j.dir.x,
            position.y + j.dir.y,
            position.z + j.dir.z,
            0, 0, 1);
}

void MCAMERA::apply(const P3D& p, const P3D& t)
{
    set_camera(p, t);
    apply();
}

void MCAMERA::apply(const P3D& position, const V3D& dir)
{
    set_camera(position, dir);
    apply();
}

void MCAMERA::apply(const V3D& X, const V3D& Y, const V3D& Z)
{
    set_vectors(X, Y, Z);
    apply();
}

void MCAMERA::apply(const float& h, const float& v, const float& b)
{
    set_angles(h, v, b);
    apply();
}

PCAMERA::PCAMERA()
{
    fovy = 90;
    aspect = 1;
    z_near = 1;
    z_far = 100;
}

PCAMERA::PCAMERA(float fovy, float aspect, float near, float far)
{
    this->fovy = fovy;
    this->aspect = aspect;
    this->z_near = near;
    this->z_far = far;
}

PCAMERA::PCAMERA(const P3D& position, const P3D& target, float fovy, float aspect, float near, float far)
    : MCAMERA(position, target)
{
    this->fovy = fovy;
    this->aspect = aspect;
    this->z_near = near;
    this->z_far = far;
}

PCAMERA::PCAMERA(const P3D& position, const V3D& dir, float fovy, float aspect, float near, float far)
    : MCAMERA(position, dir)
{
    this->fovy = fovy;
    this->aspect = aspect;
    this->z_near = near;
    this->z_far = far;
}

void PCAMERA::set_camera(float fovy)
{
    this->fovy = fovy;
}

void PCAMERA::set_camera(float fovy, float aspect)
{
    this->fovy = fovy;
    this->aspect = aspect;
}

void PCAMERA::set_camera(float fovy, float aspect, float near, float far)
{
    this->fovy = fovy;
    this->aspect = aspect;
    this->z_near = near;
    this->z_far = far;
}

void PCAMERA::apply()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, z_near, z_far);
    MCAMERA::apply();
}

void PCAMERA::apply(const P3D& position, const P3D& target)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, z_near, z_far);
    MCAMERA::apply(position, target);
}

void PCAMERA::apply(const P3D& position, const V3D& dir)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, z_near, z_far);
    MCAMERA::apply(position, dir);
}

OCAMERA::OCAMERA()
{
    left = -1;
    right = 1;
    top = 1;
    bottom = -1;
    z_near = -50;
    z_far = 50;
}

OCAMERA::OCAMERA(float left, float right, float top, float bottom, float near, float far)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    this->z_near = near;
    this->z_far = far;
}

OCAMERA::OCAMERA(const P3D& position, const P3D& target, float left, float right, float top, float bottom, float near, float far)
    : MCAMERA(position, target)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    this->z_near = near;
    this->z_far = far;
}

OCAMERA::OCAMERA(const P3D& position, const V3D& dir, float left, float right, float top, float bottom, float near, float far)
    : MCAMERA(position, dir)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    this->z_near = near;
    this->z_far = far;
}

void OCAMERA::set_camera(float left, float right, float top, float bottom)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
}

void OCAMERA::set_camera(float left, float right, float top, float bottom, float near, float far)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    this->z_near = near;
    this->z_far = far;
}

void OCAMERA::apply()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, z_near, z_far);
    MCAMERA::apply();
}

void OCAMERA::apply(const P3D& position, const P3D& target)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, z_near, z_far);
    MCAMERA::apply(position, target);
}

void OCAMERA::apply(const P3D& position, const V3D& dir)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, z_near, z_far);
    MCAMERA::apply(position, dir);
}
