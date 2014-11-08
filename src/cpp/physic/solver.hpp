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

#ifndef __SOLVER_HPP__
#define __SOLVER_HPP__

#include <list>
#include "vector.hpp"
#include "object.hpp"

namespace physic
{

class SOLVER
{
private:
    std::list<OBJECT* const> act_objs;
    std::list<OBJECT* const> dis_objs;

public:
    SOLVER();
    ~SOLVER();

    SOLVER(const SOLVER&);
    SOLVER& operator =(const SOLVER&);

    void add_object(OBJECT*);
    void enable_object(const OBJECT*);
    void disable_object(const OBJECT*);

    void update(float delta_time = 0);
};

} // namespace physic

#endif // __SOLVER_HPP__
