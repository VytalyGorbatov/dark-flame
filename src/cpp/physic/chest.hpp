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

#ifndef __CHEST_HPP__
#define __CHEST_HPP__

#include <list>
#include "object.hpp"
#include "vector.hpp"

namespace physic
{

/** Represent box with treasures. */
class CHEST : public OBJECT
{
private:
    float length;
    float width;
    float height;
public:
    CHEST();
    ~CHEST();

    void update(float delta_time, std::list<OBJECT*> objs); // corrects the objects' coordinates acording to limits
};

} // namespace physic

#endif // __CHEST_HPP__