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

#include <stdlib.h>
#include "particle.hpp"

using namespace math;
using namespace physic;

PARTICLE::PARTICLE()
{
    mass = 1;
    volume = 1;
    spin = 0;
    ttl = 0;
    f_volume = 1;
    f_spin = 0;
    angle = 0;
}

PARTICLE::PARTICLE(const P3D& position, float mass, float volume, const V3D& velocity, float spin, float ttl)
{
    if (ttl < 0) {
        ttl = 0;
    }

    this->position = position;
    this->mass = mass;
    this->volume = volume;
    this->velocity = velocity;
    this->spin = spin;
    this->ttl = ttl;

    this->f_volume = volume;
    this->f_spin = spin;
    this->angle = 0;
}

void PARTICLE::set_start(const P3D& position, float mass, float volume, const V3D& velocity, float spin, float ttl)
{
    if (ttl < 0) {
        ttl = 0;
    }

    this->position = position;
    this->mass = mass;
    this->volume = volume;
    this->velocity = velocity;
    this->spin = spin;
    this->ttl = ttl;
}

void PARTICLE::set_final(float f_volume, float f_spin, float angle)
{
    this->angle = angle;
    this->f_volume = f_volume;
    this->f_spin = f_spin;
}

bool PARTICLE::update(float dt, const float env_density, const V3D* env_force, const V3D* gravity)
{
    if (dt > ttl || !ttl) {
        ttl = 0;
        return false;
    }

    angle += spin * dt;
    float t = ttl - dt;
    float a = 1 / ttl;
    volume = (volume * t + f_volume * dt) * a;
    spin = (spin * t + f_spin * dt) * a;
    ttl -= dt;

    V3D add_v(0, 0, 0);         // additional velocity caused by external forces
    if (env_force)  {
        add_v = *env_force;
        add_v.mult_by(dt / mass);
    }

    if (gravity) {
        V3D av = *gravity;      // additional velocity caused by gravity
        av.mult_by(dt);
        if (env_density > 0) {
            V3D arh = *gravity; // Archimedes' force
            arh.mult_by(-env_density * volume * dt / mass);
            av = av + arh;
        }
        add_v = add_v + av;
    }

    position.x += (velocity.dir.x + 0.5f * add_v.dir.x) * dt;
    position.y += (velocity.dir.y + 0.5f * add_v.dir.y) * dt;
    position.z += (velocity.dir.z + 0.5f * add_v.dir.z) * dt;
    velocity = velocity + add_v;

    return true;
}

bool PARTICLE::is_alive() const
{
    return (ttl == 0) ? false : true;
}

EMITTER::EMITTER(SOLVER& world) : PHYS_OBJECT(world)
{
    particles = NULL;
    particles_cnt = 0;
    particles_cnt_max = 0;
    is_active = false;

    pps = 0;
    scale.set_xyz(1, 1, 1);
    env_density = 0;
    ext_force.dir.set_xyz(0, 0, 0);

    p_mass = 0;
    p_delta_mass = 0;
    p_volume = 0;
    p_delta_volume = 0;
    p_delta_velocity = 0;
    p_spin = 0;
    p_delta_spin = 0;
    p_ttl = 0;
    p_delta_ttl = 0;
    p_f_volume = 0;
    p_f_spin = 0;
}

EMITTER::EMITTER(SOLVER& world, const P3D& position, float pps, int particles_cnt_max) : PHYS_OBJECT(world)
{
    if (pps < 0) {
        pps = 0;
    }

    this->position = position;

    particles = NULL;
    particles_cnt = 0;
    this->particles_cnt_max = particles_cnt_max;
    is_active = false;

    this->pps = pps;
    scale.set_xyz(1, 1, 1);
    env_density = 0;
    ext_force.dir.set_xyz(0, 0, 0);

    p_mass = 0;
    p_delta_mass = 0;
    p_volume = 0;
    p_delta_volume = 0;
    p_delta_velocity = 0;
    p_spin = 0;
    p_delta_spin = 0;
    p_ttl = 0;
    p_delta_ttl = 0;
    p_f_volume = 0;
    p_f_spin = 0;
}

EMITTER::~EMITTER()
{
    for (int i = 0; i < particles_cnt; ++i) {
        delete particles[i];
    }

    delete[] particles;
}

EMITTER::EMITTER(const EMITTER& m) : PHYS_OBJECT(m)
{
    particles_cnt = m.particles_cnt;
    particles = new PARTICLE*[particles_cnt];
    for (int i = 0; i < particles_cnt; ++i) {
        particles[i] = new PARTICLE;
        *particles[i] = *m.particles[i];
    }

    scale = m.scale;
    position = m.position;
    particles_cnt_max = m.particles_cnt_max;
    pps = m.pps;
    env_density = m.env_density;
    ext_force = m.ext_force;

    p_mass = m.p_mass;
    p_volume = m.p_volume;
    p_delta_volume = m.p_delta_volume;
    p_velocity = m.p_velocity;
    p_delta_velocity = m.p_delta_velocity;
    p_spin = m.p_spin;
    p_delta_spin = m.p_delta_spin;
    p_ttl = m.p_ttl;
    p_delta_ttl = m.p_delta_ttl;

    p_f_volume = m.p_f_volume;
    p_f_spin = m.p_f_spin;
    is_active = m.is_active;
}
EMITTER& EMITTER::operator =(const EMITTER& m)
{
    if (&m == this) {
        return *this;
    }

    PHYS_OBJECT::operator =(m);

    for (int i = 0; i < particles_cnt; ++i) {
        delete particles[i];
    }
    delete[] particles;

    particles_cnt = m.particles_cnt;
    particles = new PARTICLE*[particles_cnt];
    for (int i = 0; i < particles_cnt; ++i) {
        particles[i] = new PARTICLE;
        *particles[i] = *m.particles[i];
    }

    scale = m.scale;
    position = m.position;
    particles_cnt_max = m.particles_cnt_max;
    pps = m.pps;
    env_density = m.env_density;
    ext_force = m.ext_force;

    p_mass = m.p_mass;
    p_volume = m.p_volume;
    p_delta_volume = m.p_delta_volume;
    p_velocity = m.p_velocity;
    p_delta_velocity =m.p_delta_velocity;
    p_spin = m.p_spin;
    p_delta_spin = m.p_delta_spin;
    p_ttl = m.p_ttl;
    p_delta_ttl = m.p_delta_ttl;

    p_f_volume = m.p_f_volume;
    p_f_spin = m.p_f_spin;
    is_active = m.is_active;

    return *this;
}

void EMITTER::init(const P3D& position, float pps, int particles_cnt_max)
{
    if (pps < 0) {
        pps = 0;
    }

    this->position = position;
    this->pps = pps;
}

void EMITTER::start_emission()
{
    is_active = true;
}

void EMITTER::stop_emission()
{
    is_active = false;
}

void EMITTER::update(float dt)
{
    int active_cnt = 0;
    int emitted_cnt = 0;

    PARTICLE** new_particles = NULL;
    PARTICLE** emitted_particles = NULL;

    /* calculate active particles count */
    active_cnt = particles_cnt;
    for (int i = 0; i < particles_cnt; ++i) {
        if (!particles[i]->is_alive()) {
            active_cnt--;
        }
    }

    /* calculate emitted particles count */
    if (is_active) {

        float rnd = (float)rand() / RAND_MAX; // [0..1]
        float fquantity = pps * dt;
        int iquantity = (int)fquantity; // emitted particles for this frame

        if (rnd < fquantity - iquantity) {
            iquantity++;
        }

        if (iquantity + active_cnt > particles_cnt_max) {
            iquantity = particles_cnt_max - active_cnt;
        }

        /* create new particles for this frame */
        emitted_cnt = iquantity;
        emitted_particles = new PARTICLE*[emitted_cnt];

        for (int i = 0; i < emitted_cnt; ++i) {
            emitted_particles[i] = new PARTICLE;
            init_particle(emitted_particles[i]);
        }
    }

    /* create new particles array */
    new_particles = new PARTICLE*[active_cnt + emitted_cnt];

    for (int i = 0, k = 0; i < particles_cnt; ++i) {
        if (particles[i]->is_alive()) {
            /* active particles copy to new array */
            new_particles[k++] = particles[i];
        } else {
            /* remove inactive particles */
            delete particles[i];
        }
    }

    for (int i = 0; i < emitted_cnt; ++i) {
        /* add emitted particles to array */
        new_particles[active_cnt + i] = emitted_particles[i];
    }

    /* update particles */
    delete[] particles;
    particles = new_particles;
    particles_cnt = active_cnt + emitted_cnt;

    /* computing */
    for (int i = 0; i < particles_cnt; ++i) {
        particles[i]->update(dt, env_density, &ext_force, &world->env.gravity);
    }
}

void EMITTER::init_particle(PARTICLE* p)
{
    if (!p) {
        return;
    }

    float rnd;
    float vol = p_volume;
    float spin = p_spin;
    float ttl = p_ttl;
    V3D vel = p_velocity;

    if (p_delta_volume) {
        rnd = ((float)rand() / RAND_MAX) * 2 - 1; // [-1..1]
        vol += rnd * p_delta_volume;
    }
    if (p_delta_spin) {
        rnd = ((float)rand() / RAND_MAX) * 2 - 1; // [-1..1]
        spin += rnd * p_delta_spin;
    }
    if (p_delta_ttl) {
        rnd = ((float)rand() / RAND_MAX) * 2 - 1; // [-1..1]
        ttl += rnd * p_delta_ttl;
    }

    if (p_delta_velocity) {
        rnd = ((float)rand() / RAND_MAX) * 2 - 1; // [-1..1]
        vel.dir.x += rnd * p_delta_velocity;
        rnd = ((float)rand() / RAND_MAX) * 2 - 1; // [-1..1]
        vel.dir.y += rnd * p_delta_velocity;
        rnd = ((float)rand() / RAND_MAX) * 2 - 1; // [-1..1]
        vel.dir.z += rnd * p_delta_velocity;
    }

    rnd = rand() * 3.95f;
    p->set_start(position, p_mass, vol, vel, spin, ttl);
    p->set_final(p_f_volume, p_f_spin, rnd);
}
