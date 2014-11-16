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

#include "renderer.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "primitives.hpp"
#include "vector.hpp"

using namespace renderer;
using namespace renderer::camera;
using namespace math;

void PRIMITIVES::draw_background(const TEXTURE& image, float attenuation)
{
    attenuation = attenuation > 1 ? 1 : attenuation < 0 ? 0 : attenuation;

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glColor3f(attenuation, attenuation, attenuation);
    image.bind();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 1, -1);
    glTexCoord2f(1, 0);
    glVertex3f(1, 1, -1);
    glTexCoord2f(1, 1);
    glVertex3f(1, -1, -1);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -1, -1);
    glEnd();

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void PRIMITIVES::draw_normal(const P3D& position, const V3D& direction, const float length)
{

}

void PRIMITIVES::draw_aim(const MCAMERA& camera, const TEXTURE& crest)
{

}

void PRIMITIVES::draw_skybox(const MCAMERA& camera,
        const TEXTURE& up, const TEXTURE& dn,
        const TEXTURE& ft, const TEXTURE& bk,
        const TEXTURE& lf, const TEXTURE& rt)
{

}

void PRIMITIVES::draw_cube(const P3D& c, float h)
{
    P3D d(0.7f, 0.72f, 0.2f);
    draw_cube(c, h, d);
}

void PRIMITIVES::draw_cube(const P3D& c, float h, const P3D& color)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(color.x, color.y, color.z);

    glBegin(GL_LINE_STRIP);
    glVertex3f(c.x - h, c.y - h, c.z - h);
    glVertex3f(c.x + h, c.y - h, c.z - h);
    glVertex3f(c.x + h, c.y + h, c.z - h);
    glVertex3f(c.x - h, c.y + h, c.z - h);
    glVertex3f(c.x - h, c.y - h, c.z - h);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(c.x - h, c.y - h, c.z + h);
    glVertex3f(c.x + h, c.y - h, c.z + h);
    glVertex3f(c.x + h, c.y + h, c.z + h);
    glVertex3f(c.x - h, c.y + h, c.z + h);
    glVertex3f(c.x - h, c.y - h, c.z + h);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(c.x - h, c.y - h, c.z - h);
    glVertex3f(c.x - h, c.y - h, c.z + h);
    glVertex3f(c.x + h, c.y - h, c.z - h);
    glVertex3f(c.x + h, c.y - h, c.z + h);
    glVertex3f(c.x + h, c.y + h, c.z - h);
    glVertex3f(c.x + h, c.y + h, c.z + h);
    glVertex3f(c.x - h, c.y + h, c.z - h);
    glVertex3f(c.x - h, c.y + h, c.z + h);
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
}

void PRIMITIVES::draw_box(const P3D& c, const P3D& h)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.7f, 0.72f, 0.2f);

    glBegin(GL_LINE_STRIP);
    glVertex3f(c.x      , c.y       , c.z);
    glVertex3f(c.x + h.x, c.y       , c.z);
    glVertex3f(c.x + h.x, c.y + h.y , c.z);
    glVertex3f(c.x      , c.y + h.y , c.z);
    glVertex3f(c.x      , c.y       , c.z);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(c.x      , c.y       , c.z + h.z);
    glVertex3f(c.x + h.x, c.y       , c.z + h.z);
    glVertex3f(c.x + h.x, c.y + h.y , c.z + h.z);
    glVertex3f(c.x      , c.y + h.y , c.z + h.z);
    glVertex3f(c.x      , c.y       , c.z + h.z);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(c.x      , c.y       , c.z       );
    glVertex3f(c.x      , c.y       , c.z + h.z );
    glVertex3f(c.x + h.x, c.y       , c.z       );
    glVertex3f(c.x + h.x, c.y       , c.z + h.z );
    glVertex3f(c.x + h.x, c.y + h.y , c.z       );
    glVertex3f(c.x + h.x, c.y + h.y , c.z + h.z );
    glVertex3f(c.x      , c.y + h.y , c.z       );
    glVertex3f(c.x      , c.y + h.y , c.z + h.z );
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
}

void PRIMITIVES::draw_line_strip(const P3D* pnts, int pnts_cnt, const P3D& color)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(color.x, color.y, color.z);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < pnts_cnt; ++i) {
        glVertex3f(pnts[i].x, pnts[i].y, pnts[i].z);
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
}
