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

#ifndef __PHPO_HPP__
#define __PHPO_HPP__

#include "solver.hpp"
#include "vector.hpp"

namespace physic
{

class PhPO;

struct CONNECTION {
    PhPO* object;
    float curr_length;
    float prev_length;
    float low_coeff;
    float high_coeff;
    float dumping;
};

/** Mass-spring physics model. */
class PhPO
{
private:
    float       mass;
    Math::P3D   position;
    Math::V3D   velocity;
    Math::V3D   force;
    CONNECTION* lnk;
    int         lnk_cnt;

    void add_internal_force(float dt);

public:
    PhPO();
    PhPO(float mass, Math::P3D& position, Math::V3D& velocity);
    ~PhPO();

    PhPO(const PhPO&);
    PhPO& operator =(const PhPO&);

    void init(float mass, Math::P3D& position, Math::V3D& velocity);
    void add(PhPO* object, float low_c, float high_c, float dump);
    void add_external_force(const Math::V3D& force);
    void reflect(const Math::V3D& normal, float coeff = 1);
    void friction(const Math::V3D& normal, float coeff = 1);
    Math::V3D get_impulse() const;
    Math::P3D update(float dt);

    Math::P3D get_position() const
    {
        return position;
    }
    void set_position(const Math::P3D& p)
    {
        position = p;
    }
};

} // namespace physic

#endif // __PHPO_HPP__
