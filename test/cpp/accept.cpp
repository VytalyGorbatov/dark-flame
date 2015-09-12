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
#include "help_particle.hpp"
#include "help_wave.hpp"

using namespace window;
using namespace renderer;
using namespace model;
using namespace physic;
using namespace function;

/* window settings */
const int window_width = 800;
const int window_height = 400;

/* setting up camera and viewports */
const float z_near = 0.1f;
const float z_far = 50.0f;

/* parameters for drawing objects */
math::P3D view_point(-2.1f, 2.8f, 3);
math::P3D view_point2(-0.3f, 1.2f, 0.3f);
math::P3D cube_origin(0, 0, 0);
const float cube_size = 1;

/* XXX: using global pointers to one-time allocated dynamic memory (never freed) */

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
camera::MCAMERA* camera3;

/* lights */
LIGHT* light;
math::P3D ambient(0.9f, 0.9f, 0.9f);
math::P3D diffuse(0.9f, 0.9f, 0.9f);
math::P3D specular(0.9f, 0.9f, 0.9f);
math::V3D direction(cube_origin, view_point);

/* triangle */
math::P3D p1(0, 0, 5);
math::P3D p2(0, 5, 0);
math::P3D p3(5, 0, 0);
math::P3D yellow(0.5f, 0.5f, 0);
math::P3D green(0, 1, 0);
math::TRIANGLE triangle1(p3, p2, p1);
math::TRIANGLE* triangle2;

/* static models */
MODEL_STAT* mstat;

/* physics emulating */
SOLVER world;
HELP_PARTICLE* fn_emitter;
HELP_WAVE* fn_wave;

/* init routine and loading resources */
void atest_init(void)
{
    RENDERER::init();

    viewport0 = new VIEWPORT(0, 0, window_width, window_height);
    viewport1 = new VIEWPORT(0, 0, window_width / 2, window_height);
    viewport2 = new VIEWPORT(window_width / 2, 0, window_width / 2, window_height);

    camera1 = new OCAMERA(view_point, cube_origin, -5, 5, 5, -5, z_near, z_far);
    camera2 = new PCAMERA(view_point, cube_origin, 90, (window_width / 2.0f) / window_height, z_near, z_far);
    camera3 = new PCAMERA(view_point2, cube_origin, 80, window_width / window_height, z_near, z_far);

    timer = new TIMER();
    timer->start();

    bckgnd = new TEXTURE("resources/logo.tga");

    light = new LIGHT();
    light->apply(ambient, diffuse, specular, direction);
    light->switch_off();

    mstat = new MODEL_STAT();
    mstat->init("resources/basis.cms");
    mstat->scale.set_xyz(0.03f, 0.03f, 0.03f);

    world.env.gravity.dir.set_xyz(0, 0, -9.8f);

    EMITTER ph_emitter(world, cube_origin, 10, 1000);
    fn_emitter = new HELP_PARTICLE(ph_emitter);
    fn_emitter->p_mass = 1;
    fn_emitter->p_volume = 1;
    fn_emitter->p_velocity.dir.set_xyz(0, 0, 10);
    fn_emitter->p_spin = 300;
    fn_emitter->p_ttl = 2;
    fn_emitter->p_f_volume = 1;
    fn_emitter->p_f_spin = 300;
    fn_emitter->p_delta_mass = 0.5f;
    fn_emitter->p_delta_volume = 0.2f;
    fn_emitter->p_delta_velocity = 1.5f;
    fn_emitter->p_delta_spin = 2;
    fn_emitter->p_delta_ttl = 0.3f;
    fn_emitter->start_emission();

    /* TODO: does not work yet (object coordinates) */
    math::P3D pos(0, 0, 0);
    math::P3D rot(0, 0, 0);
    math::P3D scl(1, 1, 1);
    WAVE ph_wave(world, pos, rot, scl, 32, 0.02f);
    fn_wave = new HELP_WAVE(ph_wave);

    triangle2 = triangle1.get_nested();
}

/* drawing cycle */
void atest_draw(WINDOW* wnd)
{
    float dt = timer->dt();

    /* times for every presented scene */
    static TIMER_COUNTING sc1(3);
    static TIMER_COUNTING sc2(3);
    static TIMER_COUNTING sc3(3);
    static TIMER_COUNTING sc4(3);
    static TIMER_COUNTING sc5(3);

    VIEWPORT::clear();

    if (sc1.is_active()) {

        static float att = 1.0f;
        att -=  0.5f * dt;

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

    } else if (sc2.is_active()) {

        /* triangle */
        viewport1->apply();
        camera1->apply();
        PRIMITIVES::draw_triangle(triangle1, yellow);
        /* TODO: fix nested triangle draw. */
        PRIMITIVES::draw_triangle(*triangle2, green);

        /* sphere */
        viewport2->apply();
        camera2->apply();
        math::P3D center(0, 0, 0);
        PRIMITIVES::draw_solid_sphere(center, 2.0f, yellow);

    } else if (sc3.is_active()) {

        fn_emitter->update(dt);

        /* ortho */
        viewport1->apply();
        camera1->apply();
        fn_emitter->render();

        /* perspective */
        viewport2->apply();
        camera2->apply();
        fn_emitter->render(*camera2);

    } else if (sc4.is_active()) {

        fn_wave->update(dt);

        static TIMER_ONCE rt(0.35f);
        if (rt.is_trigged()) {
            fn_wave->randomize(6);
            rt.set(0.4f);
        }

        math::P3D near_vp(view_point.x / 3, view_point.y / 3, view_point.z / 3);

        /* ortho */
        viewport1->apply();
        ((OCAMERA*)camera1)->set_camera(-1.5f, 1.5f, 1.5f, -1.5f);
        camera1->apply(near_vp, cube_origin);
        fn_wave->render();

        /* perspective */
        viewport2->apply();
        camera2->apply(near_vp, cube_origin);
        fn_wave->render();

    } else if (sc5.is_active()) {

        fn_wave->update(dt);
        fn_wave->update_random_rays(dt);

        viewport0->apply();
        camera3->apply();
        fn_wave->render();
        fn_wave->render_random_rays();

    } else {

        /* last scene */
        viewport0->apply();
        PRIMITIVES::draw_background(*bckgnd, 0.9f);
    }

    wnd->swap_buffers();
}

/* create window (pseudo code to choose OS-dependent type) */

#if defined (WINDOWS)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DFLOG.deny();

    WINDOW_WINAPI wnd;
    wnd.configure("Accept test window", window_width, window_height, hInstance);
    wnd.make_current();

    atest_init();

    int ret_code = 0;
    while (wnd.process_event(&ret_code)) {
        atest_draw(&wnd);
    }

    return ret_code;
}

#elif defined (LINUX)

int main()
{
    DFLOG.deny();

    WINDOW_X wnd;
    wnd.configure("Accept test window", window_width, window_height);
    wnd.make_current();

    atest_init();

    int ret_code = 0;
    while (wnd.process_event(&ret_code)) {
        atest_draw(&wnd);
    }

    return ret_code;
}

#else
#   error Unsupported target platform.
#endif
