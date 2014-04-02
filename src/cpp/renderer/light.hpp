#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "renderer.hpp"
#include "vector.hpp"

namespace renderer
{

class LIGHT
{
private:
    static GLenum* sources;
    int idx;

    GLenum get_source();
    void free_source(GLenum);

private:
    float ambient[4];                       // ambient RGBA intensity
    float diffuse[4];                       // diffuse RGBA intensity
    float specular[4];                      // specular RGBA intensity

    float position[4];                      // position in homogeneous object coordinates, if w != 0 then not directional
    float direction[3];                     // direction in homogeneous object coordinates

    float const_att[0];                     // attenuation factor
    float line_att[0];                      // attenuation factor
    float quad_att[0];                      // attenuation factor
    float exponent[0];                      // intensity distribution [0, 128]
    float cutoff[0];                        // maximum spread angle [0, 90]

public:
    LIGHT();
    ~LIGHT();

    /*
     * TODO:
     * Separated constructors for directional and uniform lightning (replace default constructor).
     * Methods to operate with position & direction through origin and target.
     */

    void apply();

    void switch_on() const
    {
        glEnable(sources[idx]);
    }

    void switch_off() const
    {
        glDisable(sources[idx]);
    }
};

} // namespace renderer

#endif  // __LIGHT_HPP__
