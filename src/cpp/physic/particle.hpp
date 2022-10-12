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

#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include "solver.hpp"
#include "vector.hpp"

namespace physic
{

class PARTICLE
{
public:
    Math::P3D position;
    float     angle;           // destination summary rotation angle

private:
    float     mass;
    Math::V3D velocity;
    float     volume;
    float     spin;            // positive value - clockwise, negative value - conterclock

    float     ttl;             // time to live

    float     f_volume;        // volume modificator
    float     f_spin;          // spin modificator

public:
    PARTICLE();
    PARTICLE(const Math::P3D& pos, float mass, float volume, const Math::V3D& velocity, float spin, float time_to_life);

    void set_start(const Math::P3D& pos, float mass, float volume, const Math::V3D& velocity, float spin, float time_to_life);
    void set_final(float final_volume, float final_spin, float angle = 0);

    /* if returns with false, then this particle is inactive (can be deleted) */
    bool update(float dt, const float env_density = 0, const Math::V3D* env_force = NULL, const Math::V3D* gravity = NULL);
    bool is_alive() const;
};

class EMITTER : public PHYS_OBJECT
{
protected:
    PARTICLE**  particles;
    int         particles_cnt;
    int         particles_cnt_max;
    bool        is_active;   // true - emission the particles, false - idle

private:
    void init_particle(PARTICLE*);

public:

    /* emitter properties */
    float       pps;         // particles per second;
    float       env_density;
    Math::V3D   ext_force;

    /* emission particle properties */
    float       p_mass;
    float       p_volume;
    Math::V3D   p_velocity;
    float       p_spin;
    float       p_ttl;
    float       p_f_volume;
    float       p_f_spin;
    float       p_delta_mass;
    float       p_delta_volume;
    float       p_delta_velocity;
    float       p_delta_spin;
    float       p_delta_ttl;

public:
    EMITTER();
    EMITTER(SOLVER& world);
    EMITTER(SOLVER& world, const Math::P3D& position, float particles_per_second, int max_particles);
    ~EMITTER();

    EMITTER(const EMITTER&);
    EMITTER& operator =(const EMITTER&);

    void init(const Math::P3D& position, float particles_per_second, int max_particles);
    void start_emission();
    void stop_emission();
    void update(float delta_time = 0);
    int get_dots_cnt() const;
    Math::P3D* get_dots() const;
    Math::P3D get_collision(const Math::P3D& begin, const Math::P3D& end, Math::V3D* normal = NULL) const;
};

} // namespace physic

#endif // __PARTICLE_HPP__
