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
#include "particle.hpp"

using namespace function;
using namespace physic;
using namespace renderer;
using namespace Math;

static const P3D source_position(0, 0, 2);

static void set_source(EMITTER& s)
{
    s.p_mass = 1;
    s.p_volume = 1;
    s.p_velocity.dir.set_xyz(0, 0, -1);
    s.p_spin = 3;
    s.p_ttl = 2;
    s.p_f_volume = 1;
    s.p_f_spin = 0;
    s.p_delta_mass = 0.5f;
    s.p_delta_volume = 0.2f;
    s.p_delta_velocity = 1.1f;
    s.p_delta_spin = 2;
    s.p_delta_ttl = 0.8f;
    s.ext_force.dir.set_xyz(-0.1f, 0.4f, 0.0f);
    s.start_emission();
}

HELP_WAVE::HELP_WAVE(const WAVE& m) : WAVE(m), source(*m.world, source_position, 30, 30)
{
    set_source(source);
}

HELP_WAVE::HELP_WAVE(SOLVER& world) : WAVE(world), source(world, source_position, 30, 30)
{
    set_source(source);
}

HELP_WAVE::~HELP_WAVE()
{

}

HELP_WAVE::HELP_WAVE(const HELP_WAVE& m) : WAVE(m), source(m.source)
{

}

HELP_WAVE& HELP_WAVE::operator =(const HELP_WAVE& m)
{
    if (&m == this) {
        return *this;
    }

    WAVE::operator =(m);
    source = m.source;

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

void HELP_WAVE::update_random_rays(float delta_time)
{
    source.update(delta_time);
}

void HELP_WAVE::render_random_rays(float delta_time) const
{
    P3D src_sc(0.8f, 0.85f, 0.76f);
    P3D src_ec(0.7f, 0.75f, 0.67f);
    P3D und_sc(0.3f, 0.5f, 0.45f);
    P3D und_ec(0.1f, 0.11f, 0.15f);

    P3D* pnts = source.get_dots();
    int cnt = source.get_dots_cnt();
    for (int i = 0; i < cnt; ++i) {
        P3D c = WAVE::get_collision(source.position, pnts[i]);
        PRIMITIVES::draw_line(source.position, c, src_sc, src_ec);
        PRIMITIVES::draw_line(c, pnts[i], und_sc, und_ec);
    }

    delete[] pnts;
}
