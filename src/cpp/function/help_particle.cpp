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

#include <cstddef>
#include "help_particle.hpp"
#include "particle.hpp"
#include "primitives.hpp"
#include "vector.hpp"

using namespace function;
using namespace physic;
using namespace renderer;
using namespace math;

HELP_PARTICLE::HELP_PARTICLE(const EMITTER& m) : EMITTER(m)
{

}

HELP_PARTICLE::HELP_PARTICLE(SOLVER& world) : EMITTER(world)
{

}

HELP_PARTICLE::~HELP_PARTICLE()
{

}

HELP_PARTICLE::HELP_PARTICLE(const HELP_PARTICLE& m) : EMITTER(m)
{

}

HELP_PARTICLE& HELP_PARTICLE::operator =(const HELP_PARTICLE& m)
{
    if (&m == this) {
        return *this;
    }

    EMITTER::operator =(m);

    return *this;
}

void HELP_PARTICLE::init(std::istream src)
{

}

void HELP_PARTICLE::render(float delta_time) const
{
    P3D src_color(0.3f, 0.6f, 0.9f);
    P3D prt_color(0.2f, 0.75f, 0.75f);

    PRIMITIVES::draw_cube(position, 0.08f, src_color);
    for (int i = 0; i < particles_cnt; ++i) {
        PRIMITIVES::draw_cube(particles[i]->position, 0.01f, prt_color);
    }
}

void HELP_PARTICLE::update(float delta_time)
{
    EMITTER::update(delta_time);
}
