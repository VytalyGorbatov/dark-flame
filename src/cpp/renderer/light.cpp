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

#include "logger.hpp"
#include "vector.hpp"
#include "renderer.hpp"
#include "light.hpp"

using namespace math;
using namespace renderer;

/* OpenGl guarantees 8 light sources */
#define MAX_SRCS 8

/* OpenGL IDs */
static GLenum src_list[MAX_SRCS] = {
    GL_LIGHT0,
    GL_LIGHT1,
    GL_LIGHT2,
    GL_LIGHT3,
    GL_LIGHT4,
    GL_LIGHT5,
    GL_LIGHT6,
    GL_LIGHT7
};

/* list of activity */
static bool act_list[MAX_SRCS] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
};

/* can point to other extended sources list */
GLenum* LIGHT::srcs = src_list;
bool* LIGHT::actv = act_list;
int LIGHT::srcs_cnt = MAX_SRCS;

/* if current LIGHT source is not known yet */
#define NIL -1

int LIGHT::get_source()
{
    int i = 0;
    while (i < srcs_cnt && actv[i++]);

    return i == srcs_cnt ? NIL : i;
}

void LIGHT::free_source(int idx)
{
    if (idx != NIL) {
        if (srcs && actv && idx < srcs_cnt && actv[idx]) {
            glDisable(srcs[idx]);
            actv[idx] = false;
        } else {
            DFLOG.addf("WARNING: try to free light source: %p %p [%d, max %d] %s.\n", srcs, actv, idx, srcs_cnt, actv && idx < srcs_cnt && actv[idx] ? "(in use)" : "(idle)");
        }
    }
}

LIGHT::LIGHT()
{
    idx = NIL;
}

LIGHT::~LIGHT()
{
    free_source(idx);
}

LIGHT::LIGHT(const LIGHT& m)
{
    idx = m.idx == NIL ? NIL : get_source();

    ambient[0] = m.ambient[0];
    ambient[1] = m.ambient[1];
    ambient[2] = m.ambient[2];
    ambient[3] = m.ambient[3];
    diffuse[0] = m.diffuse[0];
    diffuse[1] = m.diffuse[1];
    diffuse[2] = m.diffuse[2];
    diffuse[3] = m.diffuse[3];
    specular[0] = m.specular[0];
    specular[1] = m.specular[1];
    specular[2] = m.specular[2];
    specular[3] = m.specular[3];

    position[0] = m.position[0];
    position[1] = m.position[1];
    position[2] = m.position[2];
    position[3] = m.position[3];
    direction[0] = m.direction[0];
    direction[1] = m.direction[1];
    direction[2] = m.direction[2];

    const_att[0] = m.const_att[0];
    line_att[0] = m.line_att[0];
    quad_att[0] = m.quad_att[0];
    exponent[0] = m.exponent[0];
    cutoff[0] = m.cutoff[0];

    if (idx != NIL) {
        apply_all();
    }
}

LIGHT& LIGHT::operator =(const LIGHT& m)
{
    if (this == &m) {
        return *this;
    }

    free_source(idx);

    idx = m.idx == NIL ? NIL : get_source();

    ambient[0] = m.ambient[0];
    ambient[1] = m.ambient[1];
    ambient[2] = m.ambient[2];
    ambient[3] = m.ambient[3];
    diffuse[0] = m.diffuse[0];
    diffuse[1] = m.diffuse[1];
    diffuse[2] = m.diffuse[2];
    diffuse[3] = m.diffuse[3];
    specular[0] = m.specular[0];
    specular[1] = m.specular[1];
    specular[2] = m.specular[2];
    specular[3] = m.specular[3];

    position[0] = m.position[0];
    position[1] = m.position[1];
    position[2] = m.position[2];
    position[3] = m.position[3];
    direction[0] = m.direction[0];
    direction[1] = m.direction[1];
    direction[2] = m.direction[2];

    const_att[0] = m.const_att[0];
    line_att[0] = m.line_att[0];
    quad_att[0] = m.quad_att[0];
    exponent[0] = m.exponent[0];
    cutoff[0] = m.cutoff[0];

    if (idx != NIL) {
        apply_all();
    }

    return *this;
}

void LIGHT::apply(const P3D& ambient, const P3D& diffuse, const P3D& specular,
        const V3D& direction)
{
    this->ambient[0] = ambient.x;
    this->ambient[1] = ambient.y;
    this->ambient[2] = ambient.z;
    this->ambient[3] = 1;
    this->diffuse[0] = diffuse.x;
    this->diffuse[1] = diffuse.y;
    this->diffuse[2] = diffuse.z;
    this->diffuse[3] = 1;
    this->specular[0] = specular.x;
    this->specular[1] = specular.y;
    this->specular[2] = specular.z;
    this->specular[3] = 1;

    this->position[0] = direction.dir.x;
    this->position[1] = direction.dir.y;
    this->position[2] = direction.dir.z;
    this->position[3] = 0;

    this->direction[0] = 0;
    this->direction[1] = 0;
    this->direction[2] = 0;

    this->const_att[0] = 0;
    this->line_att[0] = 0;
    this->quad_att[0] = 0;
    this->exponent[0] = 0;
    this->cutoff[0] = 0;

    apply_all();
}

void LIGHT::apply(const P3D& ambient, const P3D& diffuse, const P3D& specular,
        const P3D& position, float const_att, float line_att, float quad_att)
{
    this->ambient[0] = ambient.x;
    this->ambient[1] = ambient.y;
    this->ambient[2] = ambient.z;
    this->ambient[3] = 1;
    this->diffuse[0] = diffuse.x;
    this->diffuse[1] = diffuse.y;
    this->diffuse[2] = diffuse.z;
    this->diffuse[3] = 1;
    this->specular[0] = specular.x;
    this->specular[1] = specular.y;
    this->specular[2] = specular.z;
    this->specular[3] = 1;

    this->position[0] = position.x;
    this->position[1] = position.y;
    this->position[2] = position.z;
    this->position[3] = 1;

    this->direction[0] = 0;
    this->direction[1] = 0;
    this->direction[2] = 0;

    this->const_att[0] = const_att;
    this->line_att[0] = line_att;
    this->quad_att[0] = quad_att;
    this->exponent[0] = 0;
    this->cutoff[0] = 180;

    apply_all();
}

void LIGHT::apply(const P3D& ambient, const P3D& diffuse, const P3D& specular,
        const P3D& position, const V3D& direction, float const_att, float line_att, float quad_att, float exponent, float cutoff)
{
    this->ambient[0] = ambient.x;
    this->ambient[1] = ambient.y;
    this->ambient[2] = ambient.z;
    this->ambient[3] = 1;
    this->diffuse[0] = diffuse.x;
    this->diffuse[1] = diffuse.y;
    this->diffuse[2] = diffuse.z;
    this->diffuse[3] = 1;
    this->specular[0] = specular.x;
    this->specular[1] = specular.y;
    this->specular[2] = specular.z;
    this->specular[3] = 1;

    this->position[0] = position.x;
    this->position[1] = position.y;
    this->position[2] = position.z;
    this->position[3] = 1;

    this->direction[0] = direction.dir.x;
    this->direction[1] = direction.dir.y;
    this->direction[2] = direction.dir.z;

    this->const_att[0] = const_att;
    this->line_att[0] = line_att;
    this->quad_att[0] = quad_att;

    if (exponent < 0) exponent = 0;
    if (exponent > 128) exponent = 128;
    this->exponent[0] = exponent;

    if (cutoff < 0) cutoff = 0;
    if (cutoff > 90) cutoff = 90;
    this->cutoff[0] = cutoff;

    apply_all();
}

void LIGHT::apply(const P3D& ambient, const P3D& diffuse, const P3D& specular,
        const P3D& origin, const P3D& target, float const_att, float line_att, float quad_att, float exponent, float cutoff)
{
    this->ambient[0] = ambient.x;
    this->ambient[1] = ambient.y;
    this->ambient[2] = ambient.z;
    this->ambient[3] = 1;
    this->diffuse[0] = diffuse.x;
    this->diffuse[1] = diffuse.y;
    this->diffuse[2] = diffuse.z;
    this->diffuse[3] = 1;
    this->specular[0] = specular.x;
    this->specular[1] = specular.y;
    this->specular[2] = specular.z;
    this->specular[3] = 1;

    this->position[0] = origin.x;
    this->position[1] = origin.y;
    this->position[2] = origin.z;
    this->position[3] = 1;

    V3D direction(origin, target);
    this->direction[0] = direction.dir.x;
    this->direction[1] = direction.dir.y;
    this->direction[2] = direction.dir.z;

    this->const_att[0] = const_att;
    this->line_att[0] = line_att;
    this->quad_att[0] = quad_att;

    if (exponent < 0) exponent = 0;
    if (exponent > 128) exponent = 128;
    this->exponent[0] = exponent;

    if (cutoff < 0) cutoff = 0;
    if (cutoff > 90) cutoff = 90;
    this->cutoff[0] = cutoff;

    apply_all();
}

void LIGHT::apply(const P3D& origin, const P3D& target)
{
    this->position[0] = origin.x;
    this->position[1] = origin.y;
    this->position[2] = origin.z;
    this->position[3] = 1;

    V3D direction(origin, target);
    this->direction[0] = direction.dir.x;
    this->direction[1] = direction.dir.y;
    this->direction[2] = direction.dir.z;

    apply_direction();
}

void LIGHT::apply_all()
{
    if (idx == NIL) {
        idx = get_source();
        if (idx == NIL) {
            return;
        }
    }

    glLightfv(srcs[idx], GL_AMBIENT, ambient);
    glLightfv(srcs[idx], GL_DIFFUSE, diffuse);
    glLightfv(srcs[idx], GL_SPECULAR, specular);

    glLightfv(srcs[idx], GL_POSITION, position);
    glLightfv(srcs[idx], GL_SPOT_DIRECTION, direction);

    glLightfv(srcs[idx], GL_CONSTANT_ATTENUATION, const_att);
    glLightfv(srcs[idx], GL_LINEAR_ATTENUATION, line_att);
    glLightfv(srcs[idx], GL_QUADRATIC_ATTENUATION, quad_att);
    glLightfv(srcs[idx], GL_SPOT_EXPONENT, exponent);
    glLightfv(srcs[idx], GL_SPOT_CUTOFF, cutoff);
}

void LIGHT::apply_direction()
{
    if (idx == NIL) {
        idx = get_source();
        if (idx == NIL) {
            return;
        }
    }
    glLightfv(srcs[idx], GL_POSITION, position);
    glLightfv(srcs[idx], GL_SPOT_DIRECTION, direction);
}

void LIGHT::switch_on() const
{
    if (idx != NIL) {
        glEnable(srcs[idx]);
    }
}

void LIGHT::switch_off() const
{
    if (idx != NIL) {
        glDisable(srcs[idx]);
    }
}

int LIGHT::get_max_sources()
{
    return srcs_cnt;
}

int LIGHT::get_sources_left()
{
    int i = 0, cnt = 0;
    while (i < srcs_cnt) {
        if (!actv[i++]) {
            cnt++;
        }
    }

    return cnt;
}
