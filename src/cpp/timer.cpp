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

void TIMER::config_timer()
{
    // TODO
}

float TIMER::get_time()
{
    return 0; // TODO
}

TIMER::TIMER()
{
    is_active = false;
    mark = 0;
    config_timer();
}

void TIMER::start()
{
    mark = get_dt();
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
