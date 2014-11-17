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

#ifndef __WAVE_HPP__
#define __WAVE_HPP__

#include "solver.hpp"
#include "vector.hpp"

namespace physic
{

namespace wave
{

struct vertex
{
    math::P3D coord;
    math::V3D norm;
    bool is_active;
};

} // namespace wave

/** Represent a liquid surface with waves. */
class WAVE : public PHYS_OBJECT
{
protected:
    wave::vertex* vertecies;
    float* field_a;
    float* field_b;
    float* prev;
    float* next;
    int dim;
    int v_cnt;
    float wh;
    float dtpf;
    float rtpf;

public:
    WAVE(SOLVER& world);
    WAVE(SOLVER& world, const math::P3D& pos, const math::P3D& rot, const math::P3D& scl, int dimension, float wave_height, float frames_per_second = 30);
    ~WAVE();

    WAVE(const WAVE&);
    WAVE& operator =(const WAVE&);

    void update(float delta_time = 0);
    void randomize(float f = 1);

    /* TODO:
    void lock_area(void*);
    void unlock_area(void*);
    void set_height(const P3D& point);
    float give_height(const P3D& point);
    */
};

} // namespace physic

#endif // __WAVE_HPP__
