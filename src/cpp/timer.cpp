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

#include "timer.hpp"
#include "logger.hpp"

#if defined (WINDOWS)

#include <windows.h>
#include <stdio.h>

using namespace std;

void TIMER::config_timer()
{
    LARGE_INTEGER freq = {0};
    LARGE_INTEGER start = {0};

    if (QueryPerformanceFrequency(&freq) && freq.QuadPart > 0) {
        QueryPerformanceCounter(&start);
        mark = start.QuadPart;
        resolution = (float)(1.0f / (double)freq.QuadPart);
    } else {
        mark = 0;
        resolution = 0;
    }
}

float TIMER::get_dt()
{
    float dtime = 0;
    LARGE_INTEGER time = {0};

    if (resolution) {
        QueryPerformanceCounter(&time);
        dtime = (float)(time.QuadPart - mark);
        mark = time.QuadPart;
    }

    return dtime;
}

#endif  // WINDOWS

#if defined LINUX

#include <time.h>

void TIMER::config_timer()
{
    timespec res;

    if (!clock_getres(CLOCK_MONOTONIC_RAW, &res) && res.tv_nsec > 0) {
        resolution = 1e-9f;
    } else if (!clock_getres(CLOCK_MONOTONIC, &res) && res.tv_nsec > 0) {
        resolution = 1e-9f;
    } else {
        mark = 0;
        resolution = 0;
    }
}

float TIMER::get_dt()
{
    float dtime = 0;
    timespec time;

    if (resolution) {
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &time)) {
            clock_gettime(CLOCK_MONOTONIC, &time);
        }
        dtime = (0 > time.tv_nsec - mark)
                ? (float)(time.tv_nsec - mark + 1000000000)
                : (float)(time.tv_nsec - mark);
        mark = time.tv_nsec;
    }

    return dtime;
}

#endif  // LINUX

TIMER::TIMER()
{
    is_active = false;
    mark = 0;
    config_timer();
}

void TIMER::start()
{
    get_dt();
    is_active = true;
}

void TIMER::stop()
{
    is_active = false;
}

float TIMER::dt()
{
    return is_active ? get_dt() * resolution : 0;
}

TIMER_COUNTING::TIMER_COUNTING(float t)
{
    ::TIMER();
    etime = t;
}

void TIMER_COUNTING::set(float t)
{
    ::TIMER();
    etime = t;
}

bool TIMER_COUNTING::is_active()
{
    if (etime > 0) {
        etime -= get_dt() * resolution;
    }

    return etime > 0;
}

TIMER_ONCE::TIMER_ONCE(float t)
{
    ::TIMER();
    etime = t;
}

void TIMER_ONCE::set(float t)
{
    ::TIMER();
    etime = t;
}

bool TIMER_ONCE::is_trigged()
{
    if (etime > 0) {
        etime -= get_dt() * resolution;
        return etime <= 0;
    }

    return false;
}
