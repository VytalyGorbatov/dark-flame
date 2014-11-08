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

#ifndef __WATER_HPP__
#define __WATER_HPP__

#include <list>
#include "object.hpp"
#include "vector.hpp"

namespace physic
{

/** Represent water as surface and included volume. */
class WATER : public OBJECT
{
private:
    math::P3D mix_xyz;                     // the nearest point to coordinates origin (bottom plane)
    math::P3D max_xyz;                     // the most far point from coordinates origin (waves' plane)

public:
    WATER();
    ~WATER();

    void update(float delta_time, std::list<OBJECT*> objs); // calculates Archimedes' force for objects
};

} // namespace physic

#endif // __WATER_HPP__
