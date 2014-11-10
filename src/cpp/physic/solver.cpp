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

#include <list>
#include <cstddef>
#include "solver.hpp"
#include "enviroment.hpp"
#include "vector.hpp"

using namespace std;
using namespace physic;
using namespace math;

list<PHYS_OBJECT*> PHYS_OBJECT::get_actors()
{
    return world->get_actors();
}

void PHYS_OBJECT::put_in_world() const
{
    world->register_object((PHYS_OBJECT*)this);
}

void PHYS_OBJECT::remove_from_world() const
{
    if (world) {
        world->unregister_object((PHYS_OBJECT*)this);
    }
}

PHYS_OBJECT::PHYS_OBJECT() : world(NULL)
{

}

PHYS_OBJECT::PHYS_OBJECT(SOLVER& w) : world(&w)
{
    position.set_xyz(0, 0, 0);
    rotation.set_xyz(0, 0, 0);
    scale.set_xyz(1, 1, 1);
    put_in_world();
}

PHYS_OBJECT::PHYS_OBJECT(SOLVER& w, const P3D& pos, const P3D& rot, const P3D& scl) : world(&w)
{
    position = pos;
    rotation = rot;
    scale = scl;
    put_in_world();
}

PHYS_OBJECT::~PHYS_OBJECT()
{
    remove_from_world();
}

void PHYS_OBJECT::collapse()
{
    *(SOLVER**)&world = NULL;
}

void SOLVER::register_object(PHYS_OBJECT*po)
{
    act_objs.push_back(po);
}

void SOLVER::unregister_object(PHYS_OBJECT* po)
{
    act_objs.remove(po);
}

void SOLVER::enable_object(PHYS_OBJECT* po)
{
    act_objs.push_back(po);
    dis_objs.remove(po);
}

void SOLVER::disable_object(PHYS_OBJECT* po)
{
    act_objs.remove(po);
    dis_objs.push_back(po);
}

SOLVER::SOLVER()
{

}

SOLVER::~SOLVER()
{
    list<PHYS_OBJECT*>::iterator i;

    for (i = act_objs.begin(); i != act_objs.end(); ++i) {
        (*i)->collapse();
    }
    for (i = dis_objs.begin(); i != dis_objs.end(); ++i) {
        (*i)->collapse();
    }
}
