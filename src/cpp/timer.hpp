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

#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "types.hpp"

class TIMER
{
private:
    bool is_active;
    int64_t mark;
    void config_timer();

protected:
    float resolution;
    float get_dt();

public:
    TIMER();

    void start();                           // start timer
    void stop();                            // stop timer
    float dt();                             // delta time elapsed since the last call (seconds)
};

class TIMER_COUNTING : public TIMER
{
private:
    float etime;

public:
    TIMER_COUNTING(float t);

    void set_time(float t);
    bool is_active();

    void start() {}
    void stop() {}
    float dt() { return 0; }
};

#endif // __TIMER_HPP__
