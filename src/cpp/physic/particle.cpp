#include <stdlib.h>
#include <math.h>
#include "particle.hpp"

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
        V3D av = *gravity;      // additional velocity coused by gravity
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
    return ttl;
}

EMITTER::EMITTER()
{
    particles = NULL;
    particles_cnt = 0;
    particles_cnt_max = 0;
    is_active = false;

    pps = 0;
    scale = 1;
    env_density = 0;

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

EMITTER::EMITTER(const P3D& position, float pps, int particles_cnt_max)
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
    scale = 1;
    env_density = 0;

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

EMITTER::EMITTER(const EMITTER& m)
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
    if (&m == this)
        return *this;

    for (int i = 0; i < particles_cnt; ++i)
        delete particles[i];
    delete[] particles;
    particles_cnt = m.particles_cnt;
    particles = new PARTICLE*[particles_cnt];
    for (int i = 0; i < particles_cnt; ++i)
    {
        particles[i] = new PARTICLE;
        *particles[i] = *m.particles[i];
    }

    scale = m.scale;
    position = m.position;
    particles_cnt_max = m.particles_cnt_max;
    pps = m.pps;
    env_density = m.env_density;

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
    int i = 0, k = 0, d = 0;

    /* remove inactive particles */
    for (i = 0; i < particles_cnt; ++i) {
        if (!particles[i]->is_alive()) {
            d++;
        }
    }

    PARTICLE** temp;
    temp = new PARTICLE*[particles_cnt - d];
    k = 0;
    for (i = 0; i < particles_cnt; ++i) {
        if (particles[i]->is_alive()) {
            temp[k++] = particles[i];
        } else {
            delete particles[i];
        }
    }

    particles_cnt -= d;
    delete[] particles;
    particles = temp;

    /* create new particles */
    if (is_active) {
        float rnd = rand() * 3.0517578125E-5; // (0..1)
        float fquantity = pps * dt;
        int iquantity = (int)fquantity; // emitted particles for this frame

        if (rnd < fquantity - iquantity) {
            iquantity++;
        }

        if (iquantity + particles_cnt > particles_cnt_max) {
            iquantity = particles_cnt_max - particles_cnt;
        }

        temp = new PARTICLE*[particles_cnt + iquantity];
        for (int i = 0; i < particles_cnt; ++i) {
            temp[i] = particles[i];
        }

        particles_cnt += iquantity;
        for (int i = particles_cnt - iquantity; i < particles_cnt; ++i) {
            temp[i] = new PARTICLE;
            init_particle(temp[i]);
        }

        delete[] particles;
        particles = temp;
    }

    /* computing */
    for (int i = 0; i < particles_cnt; ++i) {
        particles[i]->update(dt, env_density, NULL, NULL);
        // particles[i]->update(dt, env_density, &ConstForce, &CrPrm.Gravity);
    }
}

void EMITTER::init_particle(PARTICLE* p)
{
    if (!p) {
        return;
    }

    float rnd;
    float vol = p_volume;
    float p_spin = p_spin;
    float p_ttl = p_ttl;
    V3D vel = p_velocity;

    if (p_delta_volume) {
        rnd = rand() * 6.103515625E-5 - 1; // (-1..1)
        vol += rnd * p_delta_volume;
    }
    if (p_delta_spin) {
        rnd = rand() * 6.103515625E-5 - 1; // (-1..1)
        p_spin += rnd * p_delta_spin;
    }
    if (p_delta_ttl) {
        rnd = rand() * 6.103515625E-5 - 1; // (-1..1)
        p_ttl += rnd * p_delta_ttl;
    }

    if (p_delta_velocity)     {
        rnd = rand() * 6.103515625E-5 - 1;
        vel.dir.x += rnd * p_delta_velocity;
        rnd = rand() * 6.103515625E-5 - 1;
        vel.dir.y += rnd * p_delta_velocity;
        rnd = rand() * 6.103515625E-5 - 1;
        vel.dir.z += rnd * p_delta_velocity;
    }

    rnd = rand() * 3.95f;
    p->set_start(position, p_mass, vol, vel, p_spin, p_ttl);
    p->set_final(p_f_volume, p_f_spin, rnd);
}
