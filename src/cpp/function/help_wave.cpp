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

#include "help_wave.hpp"
#include "wave.hpp"
#include "primitives.hpp"
#include "vector.hpp"

using namespace function;
using namespace physic;
using namespace renderer;
using namespace math;

HELP_WAVE::HELP_WAVE(const WAVE& m) : WAVE(m)
{

}

HELP_WAVE::HELP_WAVE(SOLVER& world) : WAVE(world)
{

}

HELP_WAVE::~HELP_WAVE()
{

}

HELP_WAVE::HELP_WAVE(const HELP_WAVE& m) : WAVE(m)
{

}

HELP_WAVE& HELP_WAVE::operator =(const HELP_WAVE& m)
{
    if (&m == this) {
        return *this;
    }

    WAVE::operator =(m);

    return *this;
}

void HELP_WAVE::init(std::istream src)
{

}

void HELP_WAVE::render(float delta_time) const
{
    P3D color(0.35f, 0.7f, 0.5f);
    P3D* pnts = new P3D[dim];

    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            pnts[j] = vertecies[dim * i + j].coord;
        }

        PRIMITIVES::draw_line_strip(pnts, dim, color);

    }

    for (int j = 0; j < dim; ++j) {
        for (int i = 0; i < dim; ++i) {
            pnts[i] = vertecies[dim * i + j].coord;
        }

        PRIMITIVES::draw_line_strip(pnts, dim, color);

    }

    delete[] pnts;
}

void HELP_WAVE::update(float delta_time)
{
    WAVE::update(delta_time);
}
