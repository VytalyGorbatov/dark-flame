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

#include "arch.hpp"
#include "logger.hpp"

#include "window.hpp"
#include "window_winapi.hpp"
#include "window_x.hpp"
#include "primitives.hpp"
#include "viewport.hpp"
#include "vector.hpp"

using namespace math;
using namespace window;
using namespace renderer;

const int window_width = 800;
const int window_height = 400;

int main_test(WINDOW* wnd)
{
    wnd->make_current();

    /* parameters for drawing objects */
    P3D view_point(-2.1f, 2.8f, 3);
    P3D cube_origin(0, 0, 0);
    int cube_size(1);

    /* setting up camera and viewports */
    float z_near = 0.1f;
    float z_far = 50.0f;

    VIEWPORT viewport0(0, 0, window_width, window_height);
    VIEWPORT viewport1(0, 0, window_width / 2, window_height);
    VIEWPORT viewport2(window_width / 2, 0, window_width / 2, window_height);

    camera::MCAMERA* camera1 = new OCAMERA(view_point, cube_origin,
            -5, 5, 5, -5, z_near, z_far);
    camera::MCAMERA* camera2 = new PCAMERA(view_point, cube_origin,
            90, (window_width / 2.0f) / window_height, z_near, z_far);

    /* textures */
    TEXTURE bckgnd("resources/logo.tga");

    /* drawing cycle */
    while (1) {
        VIEWPORT::clear();

        /* draw logo */
        viewport0.apply();
        PRIMITIVES::draw_background(bckgnd, 0.5f);

        /* ortho */
        viewport1.apply();
        camera1->apply();
        PRIMITIVES::draw_cube(cube_origin, cube_size);

        /* perspective */
        viewport2.apply();
        camera2->apply();
        PRIMITIVES::draw_cube(cube_origin, cube_size);

        wnd->swap_buffers();
    }

    return 0;
}

/* create window (pseudo code to choose OS-dependent type) */

#if defined (WINDOWS)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DFLOG.deny();

    WINDOW_WINAPI wnd;
    wnd.configure("Accept test window", window_width, window_height, hInstance);

    return main_test();
}

#elif defined (LINUX)

int main()
{
    DFLOG.deny();

    WINDOW_X wnd;
    wnd.configure("Accept test window", window_width, window_height);

    return main_test(&wnd);
}

#else
#   error Unsupported target platform.
#endif
