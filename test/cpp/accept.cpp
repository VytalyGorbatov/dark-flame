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
#include "primitives.hpp"
#include "viewport.hpp"
#include "vector.hpp"

using namespace math;
using namespace window;
using namespace renderer;

WINDOW* wnd = NULL;
void *hinst = NULL;

int main_test(void);

/* create window (pseudo code to choose OS-dependent type) */

#if defined (WINDOWS)
#include "window_winapi.hpp"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WINDOW_WINAPI window_winapi;
    wnd = &window_winapi;
    hinst = hInstance;

    return main_test();
}
#elif defined (LINUX)
#include "window_x.hpp"
int main()
{
    WINDOW_X window_x;
    wnd = &window_x;
    hinst = NULL;

    return main_test();
}
#else
#   error Current operating system is not supported yet.
#endif

int main_test(void)
{
    /* comment next line to logging */
    DFLOG.deny();

    int width = 800;
    int height = 400;

    wnd->configure("Accept test window", width, height, hinst);
    wnd->make_current();

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

        wnd->swap_buffers();
    }

    return 0;
}
