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

namespace physic
{

class SOLVER;

class PHYS_OBJECT
{

/* TODO: revert back to 'protected' */
public:
    math::P3D position;                     // origin
    math::P3D rotation;                     // pitch roll yaw
    math::P3D scale;                        // scale the model
    math::V3D external_force;               // applied external force
    SOLVER* const world;                    // native world

    std::list<PHYS_OBJECT*> get_actors();
    void put_in_world() const;
    void remove_from_world() const;

public:
    PHYS_OBJECT(SOLVER& world);
    PHYS_OBJECT(SOLVER& world, const math::P3D& pos, const math::P3D& rot, const math::P3D& scl);
    virtual ~PHYS_OBJECT();

    PHYS_OBJECT(const PHYS_OBJECT&);
    PHYS_OBJECT& operator =(const PHYS_OBJECT&);

    const math::P3D& get_position() const
    {
        return position;
    }

    const math::P3D& get_rotation() const
    {
        return rotation;
    }

    const math::P3D& get_scale() const
    {
        return scale;
    }

    virtual void update(float delta_time) = 0;
    virtual math::P3D get_collision(const math::P3D& begin, const math::P3D& end, math::V3D* normal = NULL) const = 0;
    void collapse();
};

class SOLVER
{
friend class PHYS_OBJECT;

private:
    std::list<PHYS_OBJECT*> act_objs;
    std::list<PHYS_OBJECT*> dis_objs;

    std::list<PHYS_OBJECT*> get_actors()
    {
        return act_objs;
    }

    void register_object(PHYS_OBJECT*);
    void unregister_object(PHYS_OBJECT*);
    void enable_object(PHYS_OBJECT*);
    void disable_object(PHYS_OBJECT*);

public:
    ENVIROMENT env;                         // settings

    SOLVER();
    ~SOLVER();

    SOLVER(const SOLVER&);
    SOLVER& operator =(const SOLVER&);
};

} // namespace physic

#endif // __SOLVER_HPP__
