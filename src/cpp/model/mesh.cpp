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

#include <string.h>
#include "mesh.hpp"
#include "vector.hpp"
#include "geometry.hpp"
#include "renderer.hpp"

using namespace math;
using namespace renderer;
using namespace model_data;

MESH::MESH()
{
    vertex = NULL;
    normal = NULL;
    texture = NULL;
    v_cnt = 0;
    n_cnt = 0;
    t_cnt = 0;
    array_cnt = 0;
}

MESH::~MESH()
{
    delete[] vertex;
    delete[] normal;
    delete[] texture;
}

MESH::MESH(const MESH& m)
{
    v_cnt = m.v_cnt;
    n_cnt = m.n_cnt;
    t_cnt = m.t_cnt;
    array_cnt = m.array_cnt;

    vertex = new float[v_cnt];
    normal = new float[n_cnt];
    texture = new float[t_cnt];
    memcpy(vertex, m.vertex, v_cnt * sizeof(float));
    memcpy(normal, m.normal, n_cnt * sizeof(float));
    memcpy(texture, m.texture, t_cnt * sizeof(float));
}

MESH& MESH::operator =(const MESH& m)
{
    if (this == &m) {
        return *this;
    }

    delete[] vertex;
    delete[] normal;
    delete[] texture;

    v_cnt = m.v_cnt;
    n_cnt = m.n_cnt;
    t_cnt = m.t_cnt;
    array_cnt = m.array_cnt;

    vertex = new float[v_cnt];
    normal = new float[n_cnt];
    texture = new float[t_cnt];
    memcpy(vertex, m.vertex, v_cnt * sizeof(float));
    memcpy(normal, m.normal, n_cnt * sizeof(float));
    memcpy(texture, m.texture, t_cnt * sizeof(float));

    return *this;
}

void MESH::init(const P3D* vrtcs, const FACE* faces, int faces_cnt)
{
    int i, k;

    if (!vrtcs || !faces || !faces_cnt) {
        return;
    }

    v_cnt = 9 * faces_cnt;
    n_cnt = 9 * faces_cnt;
    t_cnt = 6 * faces_cnt;
    array_cnt =  3 * faces_cnt;

    delete[] vertex;
    delete[] normal;
    delete[] texture;
    vertex = new float[v_cnt];
    normal = new float[n_cnt];
    texture = new float[t_cnt];

    for (i = 0, k = 0; i < v_cnt; ++k) {
        vertex[i++] = vrtcs[faces[k].idx_a].x;
        vertex[i++] = vrtcs[faces[k].idx_a].y;
        vertex[i++] = vrtcs[faces[k].idx_a].z;
        vertex[i++] = vrtcs[faces[k].idx_b].x;
        vertex[i++] = vrtcs[faces[k].idx_b].y;
        vertex[i++] = vrtcs[faces[k].idx_b].z;
        vertex[i++] = vrtcs[faces[k].idx_c].x;
        vertex[i++] = vrtcs[faces[k].idx_c].y;
        vertex[i++] = vrtcs[faces[k].idx_c].z;
    }

    for (i = 0, k = 0; i < n_cnt; ++k) {
        normal[i++] = faces[k].norm_a.dir.x;
        normal[i++] = faces[k].norm_a.dir.y;
        normal[i++] = faces[k].norm_a.dir.z;
        normal[i++] = faces[k].norm_b.dir.x;
        normal[i++] = faces[k].norm_b.dir.y;
        normal[i++] = faces[k].norm_b.dir.z;
        normal[i++] = faces[k].norm_c.dir.x;
        normal[i++] = faces[k].norm_c.dir.y;
        normal[i++] = faces[k].norm_c.dir.z;
    }

    for (i = 0, k = 0; i < t_cnt; ++k) {
        texture[i++] = faces[k].text_a.x;
        texture[i++] = faces[k].text_a.y;
        texture[i++] = faces[k].text_b.x;
        texture[i++] = faces[k].text_b.y;
        texture[i++] = faces[k].text_c.x;
        texture[i++] = faces[k].text_c.y;
    }
}

void MESH::render() const
{
    if (vertex && normal && texture) {

        // TODO: multitexturing
        // if (CORE_ARB_MULTITEXTURE) {
        //     glClientActiveTextureARB(GL_TEXTURE1_ARB);
        //     glTexCoordPointer(2, GL_FLOAT, 0, texture);
        //     glClientActiveTextureARB(GL_TEXTURE0_ARB);
        // }

        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glTexCoordPointer(2, GL_FLOAT, 0, texture);

        glDrawArrays(GL_TRIANGLES, 0, array_cnt);
    }
}

void MESH::get_boundaries(P3D* min_xyz, P3D* max_xyz) const
{
    P3D min, max;
    int i;

    if (!min_xyz || !max_xyz) {
        return;
    }

    if (!vertex || !array_cnt) {
        *min_xyz = min;
        *max_xyz = max;
        return;
    }

    P3D a;
    a.x = vertex[0];
    a.y = vertex[1];
    a.z = vertex[2];
    min = max = a;

    for (i = 1; i < array_cnt; ++i) {
        a.x = vertex[3 * i    ];
        a.y = vertex[3 * i + 1];
        a.z = vertex[3 * i + 2];
        if (a.x < min.x)
            min.x = a.x;
        if (a.y < min.y)
            min.y = a.y;
        if (a.z < min.z)
            min.z = a.z;
        if (a.x > max.x)
            max.x = a.x;
        if (a.y > max.y)
            max.y = a.y;
        if (a.z > max.z)
            max.z = a.z;
    }

    *min_xyz = min;
    *max_xyz = max;
}

void MESH::to_sub_mesh(const P3D& centre, float half_edge)
{
    if (!array_cnt
            || !vertex || !v_cnt
            || !normal || !n_cnt
            || !texture || !t_cnt) {
        return;
    }

    P3D a, b, c;
    int i, j;

    bool* check = new bool[array_cnt];
    int c_cnt = 0;

    for (i = 0; i < array_cnt;) {
        a.x = vertex[3 * i    ];
        a.y = vertex[3 * i + 1];
        a.z = vertex[3 * i + 2];
        i++;
        b.x = vertex[3 * i    ];
        b.y = vertex[3 * i + 1];
        b.z = vertex[3 * i + 2];
        i++;
        c.x = vertex[3 * i    ];
        c.y = vertex[3 * i + 1];
        c.z = vertex[3 * i + 2];
        i++;

        if (GEOMETRY::is_triangle_in_cube(centre, half_edge, a, b, c)) {
            check[i - 2] = check[i - 1] = check[i] = true;
            c_cnt += 3;
        } else {
            check[i - 2] = check[i - 1] = check[i] = false;
        }
    }

    if (c_cnt == 0) {
        delete[] vertex;
        delete[] normal;
        delete[] texture;
        vertex = NULL;
        normal = NULL;
        texture = NULL;
        v_cnt = 0;
        n_cnt = 0;
        t_cnt = 0;
        array_cnt = 0;

        delete[] check;
        return;
    }

    float* n = new float[c_cnt * 3];
    float* v = new float[c_cnt * 3];
    float* t = new float[c_cnt * 2];

    for (i = 0; i < array_cnt; ++i) {
        if (check[i]) {
            v[3 * j    ] = vertex[3 * i    ];
            v[3 * j + 1] = vertex[3 * i + 1];
            v[3 * j + 2] = vertex[3 * i + 2];
            n[3 * j    ] = normal[3 * i    ];
            n[3 * j + 1] = normal[3 * i + 1];
            n[3 * j + 2] = normal[3 * i + 2];
            t[2 * j    ] = texture[2 * i    ];
            t[2 * j + 1] = texture[2 * i + 1];
            j++;
        }
    }

    delete[] vertex;
    delete[] normal;
    delete[] texture;
    vertex = v;
    normal = n;
    texture = t;
    v_cnt = 3 * c_cnt;
    n_cnt = 3 * c_cnt;
    t_cnt = 2 * c_cnt;
    array_cnt =  c_cnt;

    delete[] check;
}
