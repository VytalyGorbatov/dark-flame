#ifndef __WINDOW_X_HPP__
#define __WINDOW_X_HPP__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "window.hpp"

namespace window
{

class WINDOW_X : public WINDOW
{
private:
    Display *dpy;
    Window win;
    GLXContext glc;

    bool is_configured;

public:
    WINDOW_X();
    ~WINDOW_X();

    WINDOW_X(const WINDOW_X&);
    WINDOW_X& operator =(const WINDOW_X&);

    void configure(const char* name, int width, int height);
    void make_current();
    void swap_buffers();
};

} // namespace window

#endif  // __WINDOW_X_HPP__
