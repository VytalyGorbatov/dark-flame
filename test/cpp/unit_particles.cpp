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

#include "utils.hpp"
#include "particle.hpp"

using namespace math;
using namespace physic;

static void test_particle_ttl(void)
{
    P3D p(0, 0, 0);
    P3D v(0, 0, 0);
    PARTICLE a(p, 0, 0, v, 0, 10);

    ASSERT_TRUE(a.update(1));
    ASSERT_TRUE(a.is_alive());

    ASSERT_TRUE(a.update(7));
    ASSERT_TRUE(a.is_alive());

    ASSERT_TRUE(a.update(1));
    ASSERT_TRUE(a.is_alive());

    ASSERT_TRUE(!a.update(2));
    ASSERT_TRUE(!a.is_alive());

    ASSERT_TRUE(!a.update(4));
    ASSERT_TRUE(!a.is_alive());
}

static void test_emitter(void)
{
    SOLVER g;
    P3D p(1, 2, 3);
    EMITTER e(g);

    e.init(p, 100, 10000);
    e.p_ttl = 1000;

    e.update(10);
    e.update(10);
    e.update(10);

    e.start_emission();
    e.update(50);
    e.update(50);
    e.update(50);

    e.stop_emission();
    e.update(50);
    e.update(50);
    e.update(50);
}

void unit_particles(void)
{
    std::cout << __FUNCTION__ << std::endl;

    test_particle_ttl();
    test_emitter();
}
