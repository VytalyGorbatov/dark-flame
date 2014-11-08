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

#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "vector.hpp"

namespace physic
{

class OBJECT
{
public:
    math::P3D position;                     // origin
    math::P3D rotation;                     // pitch roll yaw
    math::P3D scale;                        // scale the model

public:
    OBJECT();
    virtual ~OBJECT();

    virtual void update(float time = 0) = 0;
};

} // namespace physic

#endif // __OBJECT_HPP__
