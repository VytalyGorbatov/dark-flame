/**
 * DarkFlame Copyright (C) 2014 Alexey Shumeiko, Alexey Khomyakovsky
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

#include "light.hpp"
#include "model_stat.hpp"
#include "primitives.hpp"
#include "viewport.hpp"
#include "vector.hpp"
#include "timer.hpp"

using namespace window;
using namespace renderer;
using namespace model;

/* window settings */
const int window_width = 800;
const int window_height = 400;

/* setting up camera and viewports */
const float z_near = 0.1f;
const float z_far = 50.0f;

/* parameters for drawing objects */
math::P3D view_point(-2.1f, 2.8f, 3);
math::P3D cube_origin(0, 0, 0);
const float cube_size = 1;

/* timer */
TIMER* timer;

/* textures */
TEXTURE* bckgnd;

/* viewports */
VIEWPORT* viewport0;
VIEWPORT* viewport1;
VIEWPORT* viewport2;

/* cameras */
camera::MCAMERA* camera1;
camera::MCAMERA* camera2;

/* lights */
LIGHT* light;
math::P3D ambient(0.9f, 0.9f, 0.9f);
math::P3D diffuse(0.9f, 0.9f, 0.9f);
math::P3D specular(0.9f, 0.9f, 0.9f);
math::V3D direction(cube_origin, view_point);

/* static models */
MODEL_STAT* mstat;

/* init routine and loading resources */
void init_test(void)
{
    RENDERER::init();

    viewport0 = new VIEWPORT(0, 0, window_width, window_height);
    viewport1 = new VIEWPORT(0, 0, window_width / 2, window_height);
    viewport2 = new VIEWPORT(window_width / 2, 0, window_width / 2, window_height);

    camera1 = new OCAMERA(view_point, cube_origin, -5, 5, 5, -5, z_near, z_far);
    camera2 = new PCAMERA(view_point, cube_origin, 90, (window_width / 2.0f) / window_height, z_near, z_far);

    timer = new TIMER();
    timer->start();

    bckgnd = new TEXTURE("resources/logo.tga");

    light = new LIGHT();
    light->apply(ambient, diffuse, specular, direction);
    light->switch_off();

    mstat = new MODEL_STAT();
    mstat->init("resources/basis.cms");
    mstat->scale.set_xyz(0.03f, 0.03f, 0.03f);
}

/* drawing cycle */
void main_test(WINDOW* wnd)
{
    static float att = 1.0f;
    att -= att > 0.0f ? timer->dt() * 0.2f : 0.0f;

    VIEWPORT::clear();

    /* draw logo */
    viewport0->apply();
    PRIMITIVES::draw_background(*bckgnd, att);

    /* ortho */
    viewport1->apply();
    camera1->apply();
    PRIMITIVES::draw_cube(cube_origin, cube_size);
    LIGHT::disable();
    mstat->render();

    /* perspective */
    viewport2->apply();
    camera2->apply();
    PRIMITIVES::draw_cube(cube_origin, cube_size);
    LIGHT::disable();
    mstat->render();

    wnd->swap_buffers();
}

/* create window (pseudo code to choose OS-dependent type) */

#if defined (WINDOWS)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    DFLOG.deny();

    WINDOW_WINAPI wnd;
    wnd.configure("Accept test window", window_width, window_height, hInstance);
    wnd.make_current();

    init_test();
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        main_test(&wnd);
    }

    return (int)msg.wParam;
}

#elif defined (LINUX)

int main()
{
    DFLOG.deny();

    WINDOW_X wnd;
    wnd.configure("Accept test window", window_width, window_height);
    wnd.make_current();

    init_test();
    while (1) {
        main_test(&wnd);
    }

    return 0;
}

#else
#   error Unsupported target platform.
#endif
