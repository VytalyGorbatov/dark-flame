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

#ifndef __ROD_HPP__
#define __ROD_HPP__

#include <list>
#include "solver.hpp"
#include "vector.hpp"

namespace physic
{

/** Represent a short rod. */
class ROD : public PHYS_OBJECT
{
private:
    float length;                           // length of rod

public:
    ROD();
    ~ROD();

    void update(float delta_time, std::list<PHYS_OBJECT*> objs); // checks for collisions
};

} // namespace physic

#endif // __ROD_HPP__
