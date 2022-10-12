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
#include "geometry.hpp"

using namespace Math;
using namespace physic;
using namespace physic::wave;

WAVE::WAVE(SOLVER& world) : PHYS_OBJECT(world)
{
    wh = 0;
    dtpf = 0;
    rtpf = 0;
    dim = 0;
    v_cnt = 0;
    vertecies = NULL;
    field_a = NULL;
    field_b = NULL;
    prev = NULL;
    next = NULL;
}

WAVE::WAVE(SOLVER& world, const Math::P3D& pos, const Math::P3D& rot, const Math::P3D& scl, int dimension, float wave_height, float frames_per_second) : PHYS_OBJECT(world, pos, rot, scl)
{

    if (dimension <= 0 || frames_per_second <= 0.01f) {
        wh = 0;
        dtpf = 0;
        rtpf = 0;
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
    dtpf = 1 / frames_per_second;
    rtpf = 0;
    dim = dimension;
    v_cnt = dimension * dimension;

    vertecies = new vertex[v_cnt];
    field_a = new float[v_cnt];
    field_b = new float[v_cnt];
    prev = field_a;
    next = field_b;

    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            register int idx = dim * i + j;

            vertecies[idx].coord.x = 1.0f - 2.0f * i / (dim - 1);
            vertecies[idx].coord.y = 1.0f - 2.0f * j / (dim - 1);
            vertecies[idx].coord.z = 0.0f;
            vertecies[idx].norm.dir.x = 0.0f;
            vertecies[idx].norm.dir.y = 0.0f;
            vertecies[idx].norm.dir.z = -4.0f / (dim - 1);
            vertecies[idx].is_active = true;

            field_a[idx] = 0.0f;
            field_b[idx] = 0.0f;
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
        dtpf = 0;
        rtpf = 0;
        v_cnt = 0;
        vertecies = NULL;
        field_a = NULL;
        field_b = NULL;
        prev = NULL;
        next = NULL;
        return;
    }

    wh = m.wh;
    dtpf = m.dtpf;
    rtpf = 0;
    dim = m.dim;
    v_cnt = m.v_cnt;

    vertecies = new vertex[v_cnt];
    field_a = new float[v_cnt];
    field_b = new float[v_cnt];
    memcpy(vertecies, m.vertecies, v_cnt * sizeof(vertex));
    memcpy(field_a, m.field_a, v_cnt * sizeof(float));
    memcpy(field_b, m.field_b, v_cnt * sizeof(float));

    prev = m.prev == m.field_a ? field_a : field_b;
    next = m.next == m.field_a ? field_a : field_b;
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
        dtpf = 0;
        rtpf = 0;
        v_cnt = 0;
        vertecies = NULL;
        field_a = NULL;
        field_b = NULL;
        prev = NULL;
        next = NULL;
        return *this;
    }

    wh = m.wh;
    dtpf = m.dtpf;
    rtpf = m.rtpf;
    dim = m.dim;
    v_cnt = m.v_cnt;

    vertecies = new vertex[v_cnt];
    field_a = new float[v_cnt];
    field_b = new float[v_cnt];
    memcpy(vertecies, m.vertecies, v_cnt * sizeof(vertex));
    memcpy(field_a, m.field_a, v_cnt * sizeof(float));
    memcpy(field_b, m.field_b, v_cnt * sizeof(float));

    prev = m.prev == m.field_a ? field_a : field_b;
    next = m.next == m.field_a ? field_a : field_b;

    return *this;
}

void WAVE::update(float delta_time)
{
    if (0 == dim || 0 == v_cnt) {
        return;
    }

    delta_time += rtpf;
    while (delta_time > dtpf) {

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

        delta_time -= dtpf;
    }

    rtpf = delta_time;
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

P3D WAVE::get_collision(const P3D& begin, const P3D& end, V3D* normal) const
{
    V3D n(0, 0, 0);
    V3D r(begin, end);
    P3D cp = end;

    float rl = r.get_length();

    /*
     * Surface of squads is split into array of 2 type of triangles:
     * higher and lower ABC, ACD (counterclockwise for '-z' direction)
     *
     * D       C
     *   +------+
     *   +     /+
     *   +    / +
     *   +   /  +
     *   +  /   +
     *   + /    +
     *   +------+
     * A        B
     *
     *
     * XXX: Improve array bypass.
     */

    for (int i = 0; i < dim - 1; ++i) {
        for (int j = 0; j < dim - 1; ++j) {

            register int idx = dim * i + j;
            Triangle tria(
                    vertecies[idx].coord,
                    vertecies[idx + 1].coord,
                    vertecies[idx + dim + 1].coord);

            if (tria.is_degenerate()) {
                continue;

            } else {

                P3D c = tria.get_collision(begin, end);
                V3D v(begin, c);

                /* is there a  nearer point */
                float l = v.get_length();
                if (rl <= l) {
                    continue;
                }

                rl = l;
                cp = c;
                n = tria.get_normal();
            }
        }
    }

    for (int i = 0; i < dim - 1; ++i) {
        for (int j = 0; j < dim - 1; ++j) {

            register int idx = dim * i + j;
            Triangle tria(
                    vertecies[idx].coord,
                    vertecies[idx + dim + 1].coord,
                    vertecies[idx + dim].coord);

            if (tria.is_degenerate()) {
                continue;

            } else {

                P3D c = tria.get_collision(begin, end);
                V3D v(begin, c);

                /* is there a  nearer point */
                float l = v.get_length();
                if (rl <= l) {
                    continue;
                }

                rl = l;
                cp = c;
                n = tria.get_normal();
            }
        }
    }

    if (normal) {
        *normal = n;
    }

    return cp;
}
