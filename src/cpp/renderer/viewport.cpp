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
#include "viewport.hpp"

using namespace renderer;

VIEWPORT::VIEWPORT(int x, int y, int w, int h)
{
    this->x = x > 0 ? x : 0;
    this->y = y > 0 ? y : 0;
    this->w = w > 0 ? w : 0;
    this->h = h > 0 ? h : 0;
}

void VIEWPORT::apply() const
{
    glViewport(x, y, w, h);
}

void VIEWPORT::clear()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
