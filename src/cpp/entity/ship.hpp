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

#ifndef __SHIP_HPP__
#define __SHIP_HPP__

#include "entity.hpp"

namespace entity
{

/** Represent a ship. */
class SHIP : public ENTITY
{
public:
    SHIP();
    ~SHIP();

    SHIP(const SHIP&);
    SHIP& operator =(const SHIP&);

    void init(std::istream src);
    void render(float delta_time = 0);
    void update(float delta_time = 0);
};

} // namespace entity

#endif // __SHIP_HPP__
