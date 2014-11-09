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

#if defined WINDOWS

#include <Winbase.h>

void TIMER::config_timer()
{
    int64_t freq = 0;

    if (QueryPerformanceFrequency(&freq) && freq > 0) {
        QueryPerformanceCounter(&mark);
        resolution = (float)(1.0f / (double)freq) * 1000.0f;
    } else {
        mark = 0;
        resolution = 0;
    }
}

float TIMER::get_dt()
{
    float dtime = 0;
    int64_t time = 0;
    if (resolution) {
        QueryPerformanceCounter(&time);
        dtime = (float)(time - mark);
        mark = time;
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
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
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
    return is_active ? (float)get_dt() * resolution : 0;
}
