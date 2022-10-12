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

#include <math.h>
#include <cstddef>
#include "solver.hpp"
#include "phpo.hpp"
#include "vector.hpp"

using namespace Math;
using namespace physic;

PhPO::PhPO()
{
    mass = 0;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    velocity.dir.x = 0;
    velocity.dir.y = 0;
    velocity.dir.z = 0;
    force.dir.x = 0;
    force.dir.y = 0;
    force.dir.z = 0;
    lnk = NULL;
    lnk_cnt = 0;
}

PhPO::PhPO(float m, P3D& c, V3D& v) : mass(m), position(c), velocity(v)
{
    force.dir.x = 0;
    force.dir.y = 0;
    force.dir.z = 0;
    lnk = NULL;
    lnk_cnt = 0;
}

PhPO::~PhPO()
{
    delete[] lnk;
}

PhPO::PhPO(const PhPO& m)
{
    mass = m.mass;
    position = m.position;
    velocity = m.velocity;
    force = m.force;
    lnk_cnt = m.lnk_cnt;
    lnk = new CONNECTION[lnk_cnt];
    for (int i = 0; i < lnk_cnt; ++i) {
        lnk[i] = m.lnk[i];
    }
}

PhPO& PhPO::operator =(const PhPO& m)
{
    if (&m == this) {
        return *this;
    }

    delete[] lnk;

    mass = m.mass;
    position = m.position;
    velocity = m.velocity;
    force = m.force;
    lnk_cnt = m.lnk_cnt;
    lnk = new CONNECTION[lnk_cnt];
    for (int i = 0; i < lnk_cnt; ++i) {
        lnk[i] = m.lnk[i];
    }

    return *this;
}

void PhPO::add(PhPO* obj, float low_c, float high_c, float dump)
{
    CONNECTION* temp = new CONNECTION[lnk_cnt + 1];

    for (int i = 0; i < lnk_cnt; ++i) {
        temp[i] = lnk[i];
    }

    temp[lnk_cnt].object = obj;
    temp[lnk_cnt].low_coeff = low_c;
    temp[lnk_cnt].high_coeff = high_c;
    V3D t(position, obj->position);
    temp[lnk_cnt].prev_length = t.get_length();
    temp[lnk_cnt].curr_length = temp[lnk_cnt].prev_length;
    temp[lnk_cnt].dumping = dump;

    lnk_cnt++;
    delete[] lnk;
    lnk = temp;
}

void PhPO::add_external_force(const V3D& f)
{
    force += f;
}

void PhPO::add_internal_force(float dt)
{
    for (int i = 0; i < lnk_cnt; ++i) {

        V3D r(position, lnk[i].object->position);
        float mod = r.get_length();

        if (mod > 0.000001f) {
            r.mult_by(1 / mod);
        } else {
            r.dir.set_xyz(0, 0, 0);
        }

        float c = (lnk[i].prev_length - mod) * lnk[i].dumping * mass / dt;

        V3D t(r);
        t.mult_by(c);
        lnk[i].object->force += t;
        force -= t;

        float delta = (mod - lnk[i].curr_length) / lnk[i].curr_length;

        r.mult_by(delta < 0 ? delta * lnk[i].low_coeff : delta * lnk[i].high_coeff);
        force += r;
        lnk[i].object->force -= r;

        lnk[i].prev_length = mod;
    }
}

P3D PhPO::update(float dt)
{
    force.mult_by(1 / mass);                  // a = F / m
    force.mult_by(dt);                        // a * t
    velocity += force;                        // v = v0 + a * t
    velocity.mult_by(dt);                     // s = s + v0 * t
    position = velocity.add_to(position);
    velocity.mult_by(0.5f);                 // s = s + a * t^2 / 2
    position = velocity.add_to(position);
    force.dir.set_xyz(0, 0, 0);

    return position;
}

void PhPO::init(float m, P3D& c, V3D& v)
{
    mass = m;
    position = c;
    velocity = v;
    force.dir.set_xyz(0, 0, 0);
}

void PhPO::reflect(const V3D& normal, float coeff)
{
    V3D n = velocity;
    n.projection(normal);
    V3D t = velocity - n;
    n.mult_by(coeff);
    velocity = t - n;
}

void PhPO::friction(const V3D& normal, float coeff)
{
    V3D n = velocity;
    n.projection(normal);
    V3D t = velocity - n;
    t.mult_by(coeff);
    velocity = t + n;
}

V3D PhPO::get_impulse() const
{
    V3D res = velocity;

    res.mult_by(mass);

    return res;
}
