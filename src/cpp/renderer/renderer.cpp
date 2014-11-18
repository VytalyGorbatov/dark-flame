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

#include "ogl.hpp"
#include "renderer.hpp"

using namespace renderer;

void RENDERER::init()
{

    /* basic */
    // glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH); // GL_SMOOTH or GL_FLAT

    /* materials and draw arrays */
    glEnable(GL_COLOR_MATERIAL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    /* accumulation buffer */
    // glEnable(GL_ACCUM);
    // glReadBuffer(GL_BACK);
    // glDrawBuffer(GL_BACK);

    /* multitexturing */
    // glClientActiveTextureARB(GL_TEXTURE1_ARB);
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // glClientActiveTextureARB(GL_TEXTURE0_ARB);

    /* fog */
    // glEnable(GL_FOG);
    // glFogi(GL_FOG_MODE, GL_LINEAR);
    // glFogf(GL_FOG_START, fog_start);
    // glFogf(GL_FOG_END, fog_end);
    // glFogf(GL_FOG_DENSITY, fog_density);
    // glFogfv(GL_FOG_COLOR, fog_color);
}

void RENDERER::push_matrix()
{
    glPushMatrix();
}

void RENDERER::pop_matrix()
{
    glPopMatrix();
}

void RENDERER::translate(float x, float y, float z)
{
    glTranslatef(x, y, z);
}

void RENDERER::rotate(float a, float i, float j, float k)
{
    glRotatef(a, i, j, k);
}

void RENDERER::scale(float x, float y, float z)
{
    glScalef(x, y, z);
}
