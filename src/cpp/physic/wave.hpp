#ifndef __WAVE_HPP__
#define __WAVE_HPP__

#include "vector.hpp"

namespace physic
{

namespace wave
{

struct vertex
{
    P3D coord;
    V3D norm;
    bool is_active;
};

} // namespace wave

class WAVE
{
private:
    wave::vertex* vertices;
    float* field_a;
    float* field_b;
    float* prev;
    float* next;

public:
    WAVE();
    WAVE(int dimension);

    WAVE(const WAVE&);
    WAVE& operator =(const WAVE&);

    void init(int dimension);
    void update(float dt = 0);
    void randomize();

    /* TODO:
    void lock_area(void*);
    void unlock_area(void*);
    void set_height(const P3D& point);
    float give_height(const P3D& point);
    */
};

} // namespace physic

#endif // __WAVE_HPP__
