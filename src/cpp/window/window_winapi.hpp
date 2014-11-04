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

#if defined (WINDOWS)

#ifndef __WINDOW_WINAPI_HPP__
#define __WINDOW_WINAPI_HPP__

#include "arch.hpp"
#include "window.hpp"

#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>

namespace window
{

class WINDOW_WINAPI : public WINDOW
{
private:
    HDC hdc;
    HGLRC hrc;
    HWND hwnd;
    bool is_configured;

public:
    WINDOW_WINAPI();
    ~WINDOW_WINAPI();

    WINDOW_WINAPI(const WINDOW_WINAPI&);
    WINDOW_WINAPI& operator =(const WINDOW_WINAPI&);

    void configure(const char* name, int width, int height, HINSTANCE hinst);
    void make_current();
    void swap_buffers();
};

} // namespace window

#endif  // __WINDOW_WINAPI_HPP__

#endif  // WINDOWS
