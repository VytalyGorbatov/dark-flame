#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include "vector.hpp"

class PARTICLE
{
private:
    P3D     position;
    float   mass;
    V3D     velocity;
    float   volume;
    float   spin;            // positive value - clockwise, negative value - conterclock
    float   angle;           // destination summary rotation angle

    float   ttl;             // time to live

    float   f_volume;        // volume modificator
    float   f_spin;          // spin modificator

public:
    PARTICLE();
    PARTICLE(const P3D& pos, float mass, float volume, const V3D& velocity, float spin, float time_to_life);

    void set_start(const P3D& pos, float mass, float volume, const V3D& velocity, float spin, float time_to_life);
    void set_final(float final_volume, float final_spin, float angle = 0);

    /* if returns with false, then this particle is inactive (can be deleted) */
    bool update(float dt, const float env_density = 0, const V3D* env_force = NULL, const V3D* gravity = NULL);
    bool is_alive() const;
};

class EMITTER
{
private:
    PARTICLE**  particles;
    int         particles_cnt;
    int         particles_cnt_max;
    bool        is_active;   // true - emission the particles, false - idle

private:
    void init_particle(PARTICLE*);

public:

    /* emitter properties */
    P3D         position;

    float       pps;         // particles per second;
    float       scale;
    float       env_density;
    V3D         ext_force;

    /* emission particle properties */
    float       p_mass;
    float       p_volume;
    V3D         p_velocity;
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
    EMITTER(const P3D& position, float particles_per_second, int max_particles);
    ~EMITTER();

    EMITTER(const EMITTER&);
    EMITTER& operator =(const EMITTER&);

    void init(const P3D& position, float particles_per_second, int max_particles);
    void start_emission();
    void stop_emission();
    void update(float dt);
};

#endif // __PARTICLE_HPP__
