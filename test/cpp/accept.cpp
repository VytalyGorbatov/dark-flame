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
#include "window_x.hpp"
#include "primitives.hpp"
#include "viewport.hpp"
#include "vector.hpp"

using namespace math;
using namespace window;
using namespace renderer;

int main()
{

    /* comment next line to logging */
    DFLOG.deny();

    /* create window (pseudo code to choose OS-dependent type) */
    int width = 800;
    int height = 400;

    WINDOW_X window_x;
    WINDOW* window = &window_x;
    window->configure("Accept test window", width, height);
    window->make_current();

    /* parameters for drawing objects */
    P3D view_point(-2.1f, 2.8f, 3);
    P3D cube_origin(0, 0, 0);
    int cube_size(1);

    /* setting up camera and viewports */
    float z_near = 0.1f;
    float z_far = 50.0f;
    VIEWPORT viewport1(0, 0, width / 2, height);
    VIEWPORT viewport2(width / 2, 0, width / 2, height);
    camera::MCAMERA* camera1 = new OCAMERA(view_point, cube_origin, -5, 5, 5, -5, z_near, z_far);
    camera::MCAMERA* camera2 = new PCAMERA(view_point, cube_origin, 90, (width / 2.0f) / height, z_near, z_far);

    /* drawing cycle */
    while (1) {
        VIEWPORT::clear();

        /* ortho */
        viewport1.apply();
        camera1->apply();
        PRIMITIVES::draw_cube(cube_origin, cube_size);

        /* perspective */
        viewport2.apply();
        camera2->apply();
        PRIMITIVES::draw_cube(cube_origin, cube_size);

        window->swap_buffers();
    }

    return 0;
}
