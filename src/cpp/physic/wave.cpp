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

#include <string.h>
#include <stdlib.h>
#include "wave.hpp"

using namespace math;
using namespace physic;
using namespace physic::wave;

WAVE::WAVE(SOLVER& world) : PHYS_OBJECT(world)
{
    wh = 0;
    dim = 0;
    v_cnt = 0;
    vertecies = NULL;
    field_a = NULL;
    field_b = NULL;
    prev = NULL;
    next = NULL;
}

WAVE::WAVE(SOLVER& world, const math::P3D& pos, const math::P3D& rot, const math::P3D& scl, int dimension, float wave_height) : PHYS_OBJECT(world, pos, rot, scl)
{

    if (dimension <= 0) {
        wh = 0;
        dim = 0;
        v_cnt = 0;
        vertecies = NULL;
        field_a = NULL;
        field_b = NULL;
        prev = NULL;
        next = NULL;
        return;
    }

    wh = wave_height < 1 ? wave_height > 0 ? wave_height : 0 : 1;
    dim = dimension;
    v_cnt = dimension * dimension;

    vertecies = new vertex[v_cnt];
    field_a = new float[v_cnt];
    field_b = new float[v_cnt];
    prev = field_a;
    next = field_b;

    memset(vertecies, 0, v_cnt * sizeof(vertex));
    memset(field_a, 0, v_cnt * sizeof(float));
    memset(field_b, 0, v_cnt * sizeof(float));
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++i) {
            vertecies[dim * i + j].coord.x = 1.0f - 2.0f * i / (dim - 1);
            vertecies[dim * i + j].coord.y = 1.0f - 2.0f * j / (dim - 1);
            vertecies[dim * i + j].norm.dir.z = -4.0f / 127.0f;
            vertecies[dim * i + j].is_active = true;
        }
    }
}

WAVE::~WAVE()
{
    delete[] vertecies;
    delete[] field_a;
    delete[] field_b;
}

WAVE::WAVE(const WAVE& m) : PHYS_OBJECT(m)
{
    if (m.v_cnt <=0) {
        wh = 0;
        v_cnt = 0;
        vertecies = NULL;
        field_a = NULL;
        field_b = NULL;
        prev = NULL;
        next = NULL;
        return;
    }

    wh = m.wh;
    dim = m.dim;
    v_cnt = m.v_cnt;

    vertecies = new vertex[v_cnt];
    field_a = new float[v_cnt];
    field_b = new float[v_cnt];
    memcpy(vertecies, m.vertecies, v_cnt * sizeof(vertex));
    memcpy(field_a, m.field_a, v_cnt * sizeof(float));
    memcpy(field_b, m.field_b, v_cnt * sizeof(float));

    prev = m.prev;
    next = m.next;
}

WAVE& WAVE::operator =(const WAVE& m)
{
    if (&m == this) {
        return *this;
    }

    PHYS_OBJECT::operator =(m);

    delete[] vertecies;
    delete[] field_a;
    delete[] field_b;

    if (m.v_cnt <=0) {
        wh = 0;
        v_cnt = 0;
        vertecies = NULL;
        field_a = NULL;
        field_b = NULL;
        prev = NULL;
        next = NULL;
        return *this;
    }

    wh = m.wh;
    dim = m.dim;
    v_cnt = m.v_cnt;

    vertecies = new vertex[v_cnt];
    field_a = new float[v_cnt];
    field_b = new float[v_cnt];
    memcpy(vertecies, m.vertecies, v_cnt * sizeof(vertex));
    memcpy(field_a, m.field_a, v_cnt * sizeof(float));
    memcpy(field_b, m.field_b, v_cnt * sizeof(float));

    prev = m.prev;
    next = m.next;

    return *this;
}

void WAVE::update(float delta_time)
{
    if (0 == dim || 0 == v_cnt || 0 == delta_time) {
        return;
    }

    for (int i = 1; i < dim - 1; ++i) {
        for (int j = 1; j < dim - 1; ++j) {
            register int idx = dim * i + j;

            vertecies[idx].coord.z = next[idx];
            vertecies[idx].norm.dir.x = next[idx - dim] - next[idx + dim];
            vertecies[idx].norm.dir.y = next[idx - 1] - next[idx + 1];

            if (vertecies[idx].is_active) {
                float laplas = -next[idx]
                        + 0.175f
                        * (next[idx - dim] + next[idx + dim] + next[idx + 1] + next[idx - 1])
                        + 0.075f
                        * (next[idx - dim - 1] + next[idx - dim + 1] + next[idx + dim - 1] + next[idx + dim + 1]);

                prev[idx] = (2.0f - wh) * next[idx] - (1.0f - wh) * prev[idx] + laplas;

            } else {

                prev[idx] = 0;
            }
        }
    }

    float* sw = prev; prev = next; next = sw;
}

void WAVE::randomize(float f)
{
    if (0 == dim || 0 == v_cnt) {
        return;
    }

    int ri = rand() % dim;
    int rj = rand() % dim;

    next[dim * ri + rj] -= wh * f;
}
