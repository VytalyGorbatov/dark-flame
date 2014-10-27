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

#ifndef __SIGNAL_HPP__
#define __SIGNAL_HPP__

namespace ai
{

/**
 * Special type to represent signal, parameters value.
 */
class SIGNAL
{
public:
    static const int max;                   // maximum value
    static const int min;                   // minimum value
    int value;                              // value of signal or parameter

public:
    SIGNAL() : value(0) {}
    SIGNAL(int v) : value(v) {}

    SIGNAL operator -()
    {
        SIGNAL s(-value);
        return s;
    }
};

inline SIGNAL operator +(SIGNAL lh, const SIGNAL& rh)
{
    int r = lh.value + rh.value;
    lh.value = r > lh.value ? r : SIGNAL::max;
    return lh;
}

inline SIGNAL operator -(SIGNAL lh, const SIGNAL& rh)
{
    int r = lh.value - rh.value;
    lh.value = r < lh.value ? r : SIGNAL::min;
    return lh;
}

/**
 * Special type to represent potential, influence level.
 */
class LEVEL
{
public:
    static const float max;                 // ceil value
    static const float min;                 // floor value
    float value;                            // normalized value

public:
    LEVEL() : value(0) {}
    LEVEL(float v) : value(v) {}
};

} // namespace ai

#endif // __SIGNAL_HPP__
