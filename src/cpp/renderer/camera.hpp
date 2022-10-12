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

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "vector.hpp"

namespace renderer
{

namespace camera
{

/* rules with modelview permutations */
class MCAMERA
{
protected:
    Math::P3D position;
    Math::V3D i;
    Math::V3D j;
    Math::V3D k;

    float pitch;
    float roll;
    float yaw;

    void set_camera(const Math::P3D& position, const Math::P3D& target);
    void set_camera(const Math::P3D& position, const Math::V3D& dir);
    void set_vectors(const Math::V3D& i, const Math::V3D& j, const Math::V3D& k);
    void set_angles(const float pitch, const float roll, const float yaw);

public:
    MCAMERA();
    MCAMERA(const Math::P3D& position);
    MCAMERA(const Math::P3D& position, const Math::P3D& target);
    MCAMERA(const Math::P3D& position, const Math::V3D& dir);

    virtual void apply();
    virtual void apply(const Math::P3D& position, const Math::P3D& target);
    virtual void apply(const Math::P3D& position, const Math::V3D& dir);
    virtual void apply(const Math::V3D& i, const Math::V3D& j, const Math::V3D& k);
    virtual void apply(const float& pitch, const float& roll, const float& yaw);

    Math::P3D get_position() const
    {
        return position;
    }

    Math::V3D get_i() const
    {
        return i;
    }

    Math::V3D get_j() const
    {
        return j;
    }

    Math::V3D get_k() const
    {
        return k;
    }
};

} //namespace camera

/* rules with projection permutations (perspective) */
class PCAMERA : public camera::MCAMERA
{
protected:
    float fovy;
    float aspect;
    float z_near;
    float z_far;

public:
    PCAMERA();
    PCAMERA(float fovy, float aspect, float near, float far);
    PCAMERA(const Math::P3D& position, const Math::P3D& target, float fovy, float aspect, float near, float far);
    PCAMERA(const Math::P3D& position, const Math::V3D& dir, float fovy, float aspect, float near, float far);

    void set_camera(float fovy);
    void set_camera(float fovy, float aspect);
    void set_camera(float fovy, float aspect, float near, float far);

    void apply();
    void apply(const Math::P3D& position, const Math::P3D& target);
    void apply(const Math::P3D& position, const Math::V3D& dir);
};

/* rules with projection permutations (ortho) */
class OCAMERA : public camera::MCAMERA
{
protected:
    float left;
    float right;
    float top;
    float bottom;
    float z_near;
    float z_far;

public:
    OCAMERA();
    OCAMERA(float left, float right, float top, float bottom, float near, float far);
    OCAMERA(const Math::P3D& position, const Math::P3D& target, float left, float right, float top, float bottom, float near, float far);
    OCAMERA(const Math::P3D& position, const Math::V3D& dir, float left, float right, float top, float bottom, float near, float far);

    void set_camera(float left, float right, float top, float bottom);
    void set_camera(float left, float right, float top, float bottom, float near, float far);

    void apply();
    void apply(const Math::P3D& position, const Math::P3D& target);
    void apply(const Math::P3D& position, const Math::V3D& dir);
};

} // namespace renderer

#endif  // __CAMERA_HPP__
