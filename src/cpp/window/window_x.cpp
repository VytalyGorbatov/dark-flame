#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "logger.hpp"
#include "window_x.hpp"

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

void WINDOW_X::configure()
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

    win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    XMapWindow(dpy, win);
    XStoreName(dpy, win, "VERY SIMPLE APPLICATION");

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
