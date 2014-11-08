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
#include "enviroment.hpp"
#include "vector.hpp"
#include "object.hpp"

namespace physic
{

class SOLVER
{
public:
    std::list<PHYS_OBJECT* const> act_objs;
    std::list<PHYS_OBJECT* const> dis_objs;

    void register_object(PHYS_OBJECT*);
    void unregister_object(const PHYS_OBJECT*);
    void enable_object(const PHYS_OBJECT*);
    void disable_object(const PHYS_OBJECT*);

public:
    ENVIROMENT env;                         // settings

    SOLVER();
    ~SOLVER();

    SOLVER(const SOLVER&);
    SOLVER& operator =(const SOLVER&);
};

} // namespace physic

#endif // __SOLVER_HPP__
