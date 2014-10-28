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

#ifndef __WINDOW_X_HPP__
#define __WINDOW_X_HPP__

#include "arch.hpp"
#include "window.hpp"

#if defined (LINUX)

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

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

    void configure(const char* name, int width, int height, void* hinst);
    void make_current();
    void swap_buffers();
};

} // namespace window

#endif // LINUX

#endif  // __WINDOW_X_HPP__
