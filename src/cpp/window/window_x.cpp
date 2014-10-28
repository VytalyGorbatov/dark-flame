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
#include "window_x.hpp"

#if defined (LINUX)

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

using namespace window;

WINDOW_X::WINDOW_X()
{
    is_configured = false;
}

WINDOW_X::~WINDOW_X()
{
    if (is_configured) {
        glXMakeCurrent(dpy, None, NULL);
        glXDestroyContext(dpy, glc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }
}

WINDOW_X::WINDOW_X(const WINDOW_X& m)
{
    is_configured = false;
}

WINDOW_X& WINDOW_X::operator =(const WINDOW_X& m)
{
    if (this == &m) {
        return *this;
    }

    if (is_configured) {
        glXMakeCurrent(dpy, None, NULL);
        glXDestroyContext(dpy, glc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }

    is_configured = false;

    return *this;
}

void WINDOW_X::configure(const char* name, int width, int height, void* hinst)
{
    int att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

    if (is_configured) {
        glXMakeCurrent(dpy, None, NULL);
        glXDestroyContext(dpy, glc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }

    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        DFLOG.addf("WINDOW_X: cannot connect to X server\n");
    }

    Window root = DefaultRootWindow(dpy);

    XVisualInfo* vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
        DFLOG.addf("WINDOW_X: no appropriate visual found\n");
    } else {
        DFLOG.addf("WINDOW_X: visual %p selected\n", (void *)vi->visualid);
    }

    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.event_mask = ExposureMask | KeyPressMask;

    win = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XMapWindow(dpy, win);
    XStoreName(dpy, win, name);

    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);

    is_configured = true;
}

void WINDOW_X::make_current()
{
    if (is_configured) {
        glXMakeCurrent(dpy, win, glc);
    } else {
        DFLOG.addf("WINDOW_X: try to make current uninited context\n");
    }
}

void WINDOW_X::swap_buffers()
{
    glXSwapBuffers(dpy, win);
}

#endif //LINUX
