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

#ifndef __VIEWPORT_HPP__
#define __VIEWPORT_HPP__

namespace renderer
{

class VIEWPORT
{
private:
    int x;
    int y;
    int w;
    int h;

public:
    VIEWPORT(int x_left_bot, int y_left_bot, int width, int height);

    void apply() const;

    static void clear();
};

} // namespace renderer

#endif  // __VIEWPORT_HPP__
