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

#ifndef __OGL_HPP__
#define __OGL_HPP__

/*
 * In order to speed up the development stage,
 * render through OpenGL directly.
 */
#include "arch.hpp"

#if defined (WINDOWS)

#include <windows.h>
#undef near
#undef far

#elif defined (LINUX)

#include <GL/glut.h>

#else
#   error Unsupported target platform.
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#endif // __OGL_HPP__
