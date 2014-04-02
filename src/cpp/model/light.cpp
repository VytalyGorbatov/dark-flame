#include "vector.hpp"
#include "renderer.hpp"
#include "light.hpp"

using namespace math;
using namespace renderer;
using namespace model;

GLenum* LIGHT::sources = NULL;

/* TODO */
GLenum LIGHT::get_source() { return -1; }
void LIGHT::free_source(GLenum) {}

LIGHT::LIGHT()
{
    /* TODO: set uniform or directional lightning */

    idx = get_source();
}

LIGHT::~LIGHT()
{
    free_source(idx);
}

void LIGHT::apply()
{
    glLightfv(sources[idx], GL_AMBIENT, ambient);
    glLightfv(sources[idx], GL_DIFFUSE, diffuse);
    glLightfv(sources[idx], GL_SPECULAR, specular);

    glLightfv(sources[idx], GL_POSITION, position);
    glLightfv(sources[idx], GL_SPOT_DIRECTION, direction);

    glLightfv(sources[idx], GL_CONSTANT_ATTENUATION, const_att);
    glLightfv(sources[idx], GL_LINEAR_ATTENUATION, line_att);
    glLightfv(sources[idx], GL_QUADRATIC_ATTENUATION, quad_att);
    glLightfv(sources[idx], GL_SPOT_EXPONENT, exponent);
    glLightfv(sources[idx], GL_SPOT_CUTOFF, cutoff);
}
