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

#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "solver.hpp"
#include "vector.hpp"

namespace physic
{

/** Represent objects as spheres. */
class SPHERE : public PHYS_OBJECT
{
protected:
    float mass;
    float radius;
    float volume;

public:
    SPHERE(SOLVER& world);
    SPHERE(SOLVER& world, const Math::P3D& pos, const Math::P3D& rot, const Math::P3D& scl, float mass, float radius)
    ~SPHERE();

    SPHERE(const SPHERE&);
    SPHERE& operator =(const SPHERE&);

    void update(float delta_time = 0);
    Math::P3D get_collision(const Math::P3D& begin, const Math::P3D& end, Math::V3D* normal = NULL);
};

} // namespace physic

#endif // __SPHERE_HPP__
