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

#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include <istream>
#include "model.hpp"
#include "phys_object.hpp"

/**
 * Simple multiply or once-triggered actions.
 */
class FUNCTION
{
protected:
    model::MODEL* model;
    physics::PHYS_OBJECT* object;

    /* Some interface to receive events from others. */

public:
    virtual ~FUNCTION() {}

    virtual void init(std::istream src) = 0;
    virtual void render(float delta_time = 0) const = 0;
    virtual void update(float delta_time = 0) = 0;
};

#endif // __FUNCTION_HPP__
