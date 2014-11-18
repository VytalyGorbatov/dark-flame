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
#include <fstream>
#include "model_stat.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "vector.hpp"
#include "geometry.hpp"
#include "renderer.hpp"
#include "logger.hpp"

using namespace std;
using namespace math;
using namespace model;
using namespace renderer;

const int MODELVERSION = 2;

MODEL_STAT::MODEL_STAT()
{
    P3D N(0, 0, 0);
    P3D E(1, 1, 1);

    materials = NULL;
    meshes = NULL;
    materials_cnt = 0;
    meshes_cnt = 0;
    position = N;
    rotation = N;
    scale = E;
}

MODEL_STAT::~MODEL_STAT()
{
    delete[] materials;
    delete[] meshes;
}

MODEL_STAT::MODEL_STAT(const MODEL_STAT& m)
{
    position = m.position;
    rotation = m.rotation;
    scale = m.scale;

    materials_cnt = m.materials_cnt;
    meshes_cnt = m.meshes_cnt;

    materials = new MATERIAL[materials_cnt];
    meshes = new MESH[meshes_cnt];
    memcpy(materials, m.materials, materials_cnt * sizeof(MATERIAL));
    memcpy(meshes, m.meshes, meshes_cnt * sizeof(MESH));
}

MODEL_STAT& MODEL_STAT::operator =(const MODEL_STAT& m)
{
    if (this == &m) {
        return *this;
    }

    delete[] materials;
    delete[] meshes;

    position = m.position;
    rotation = m.rotation;
    scale = m.scale;

    materials_cnt = m.materials_cnt;
    meshes_cnt = m.meshes_cnt;

    materials = new MATERIAL[materials_cnt];
    meshes = new MESH[meshes_cnt];
    memcpy(materials, m.materials, materials_cnt * sizeof(MATERIAL));
    memcpy(meshes, m.meshes, meshes_cnt * sizeof(MESH));

    return *this;
}

int MODEL_STAT::enum_polygons() const
{
    int result = 0;

    for (int i = 0; i < meshes_cnt; ++i) {
        result += meshes[i].enum_polygons();
    }

    return result;
}

void MODEL_STAT::to_sub_model(const P3D& centre, float half_edge)
{
    for (int i = 0; i < meshes_cnt; ++i) {
        meshes[i].to_sub_mesh(centre, half_edge);
    }
}

void MODEL_STAT::render(float delta_time) const
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(-rotation.z, 0, 0, 1);
    glRotatef(-rotation.y, 0, 1, 0);
    glRotatef(-rotation.x, 1, 0, 0);
    glScalef(scale.x, scale.y, scale.z);

    for (int i = 0; i < materials_cnt; ++i) {
        materials[i].texture.bind();
        glMaterialfv(GL_FRONT, GL_AMBIENT, materials[i].ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[i].diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, materials[i].specular);
        glMaterialf(GL_FRONT, GL_SHININESS, materials[i].shine);

        for (int j = 0; j < meshes_cnt; ++j) {
            if (meshes[j].m_idx == i) {
                meshes[j].render();
            }
        }
    }

    glPopMatrix();
}

void MODEL_STAT::update(float delta_time)
{

}

void MODEL_STAT::init(const char* file_path)
{
    DFLOG.addf("MODEL_STAT.init: reading from %s\n", file_path);
    ::ifstream file(file_path, ios::in | ios::binary);
    if (file.fail()) {
        DFLOG.addf("MODEL_STAT.init: can't find resource %s\n", file_path);
        return;
    }

    int ver;
    file.read((char*)&ver, sizeof(ver));
    if (ver != MODELVERSION) {
        DFLOG.addf("Discrepancy of the versions (inquired %d, distributed %d)\n", MODELVERSION, ver);
        return;
    }

    file.read((char*)&materials_cnt, sizeof(materials_cnt));
    if (materials_cnt <= 0) {
        DFLOG.addf("Value: incorrect materials_cnt (%d)\n\tfile:  %s\n", materials_cnt, file_path);
        return;
    }
    delete[] materials;
    materials = new MATERIAL[materials_cnt];
    string* mtn = new string[materials_cnt];

    file.read((char*)&meshes_cnt, sizeof(meshes_cnt));
    if (meshes_cnt <= 0) {
        DFLOG.addf("Value: incorrect meshes_cnt (%d)\n\tfile:  %s\n", materials_cnt, file_path);
        delete[] mtn;
        return;
    }
    delete[] meshes;
    meshes = new MESH[meshes_cnt];

    for (int i = 0; i < materials_cnt; ++i) {
        int name_len;
        char* name;

        file.read((char*)&name_len, sizeof(name_len));
        if (name_len <= 0) {
            DFLOG.addf("Value: incorrect name_len (%d)\n\tfile: %s\n", name_len, file_path);
            delete[] mtn;
            return;
        }
        name = new char[name_len + 1];
        name[name_len] = '\0';
        file.read(name, name_len);
        mtn[i] = name;
        delete[] name;

        file.read((char*)&materials[i].ambient[0], sizeof(materials[i].ambient[0]));
        file.read((char*)&materials[i].ambient[1], sizeof(materials[i].ambient[1]));
        file.read((char*)&materials[i].ambient[2], sizeof(materials[i].ambient[2]));
        file.read((char*)&materials[i].ambient[3], sizeof(materials[i].ambient[3]));
        file.read((char*)&materials[i].diffuse[0], sizeof(materials[i].diffuse[0]));
        file.read((char*)&materials[i].diffuse[1], sizeof(materials[i].diffuse[1]));
        file.read((char*)&materials[i].diffuse[2], sizeof(materials[i].diffuse[2]));
        file.read((char*)&materials[i].diffuse[3], sizeof(materials[i].diffuse[3]));
        file.read((char*)&materials[i].specular[0], sizeof(materials[i].specular[0]));
        file.read((char*)&materials[i].specular[1], sizeof(materials[i].specular[1]));
        file.read((char*)&materials[i].specular[2], sizeof(materials[i].specular[2]));
        file.read((char*)&materials[i].specular[3], sizeof(materials[i].specular[3]));
        file.read((char*)&materials[i].shine, sizeof(materials[i].shine));
    }

    for (int i = 0; i < meshes_cnt; ++i) {
        P3D* vrtcs;
        FACE* faces;
        int vrtcs_cnt;
        int faces_cnt;

        file.read((char*)&vrtcs_cnt, sizeof(vrtcs_cnt));
        if (vrtcs_cnt <= 0) {
            DFLOG.addf("Value: incorrect vertecies count (%d)\n\tfile:  %s\n", vrtcs_cnt, file_path);
            delete[] mtn;
            return;
        }
        vrtcs = new P3D[vrtcs_cnt];

        file.read((char*)&faces_cnt, sizeof(faces_cnt));
        if (faces_cnt <= 0) {
            DFLOG.addf("Value: incorrect faces count (%d)\n\tfile:  %s\n", faces_cnt, file_path);
            delete[] mtn;
            return;
        }
        faces = new FACE[faces_cnt];

        file.read((char*)&meshes[i].m_idx, sizeof(meshes[i].m_idx));
        if (meshes[i].m_idx < 0) {
            DFLOG.addf("Value: incorrect material index (%d)\n\tfile:  %s\n", meshes[i].m_idx, file_path);
            delete[] mtn;
            return;
        }

        for (int j = 0; j < vrtcs_cnt; ++j) {
            file.read((char*)&vrtcs[j].x, sizeof(vrtcs[j].x));
            file.read((char*)&vrtcs[j].y, sizeof(vrtcs[j].y));
            file.read((char*)&vrtcs[j].z, sizeof(vrtcs[j].z));
        }

        for (int j = 0; j < faces_cnt; ++j) {
            file.read((char*)&faces[j].idx_a, sizeof(faces[j].idx_a));
            file.read((char*)&faces[j].idx_b, sizeof(faces[j].idx_b));
            file.read((char*)&faces[j].idx_c, sizeof(faces[j].idx_c));
            file.read((char*)&faces[j].norm_a.dir.x, sizeof(faces[j].norm_a.dir.x));
            file.read((char*)&faces[j].norm_a.dir.y, sizeof(faces[j].norm_a.dir.y));
            file.read((char*)&faces[j].norm_a.dir.z, sizeof(faces[j].norm_a.dir.z));
            file.read((char*)&faces[j].norm_b.dir.x, sizeof(faces[j].norm_b.dir.x));
            file.read((char*)&faces[j].norm_b.dir.y, sizeof(faces[j].norm_b.dir.y));
            file.read((char*)&faces[j].norm_b.dir.z, sizeof(faces[j].norm_b.dir.z));
            file.read((char*)&faces[j].norm_c.dir.x, sizeof(faces[j].norm_c.dir.x));
            file.read((char*)&faces[j].norm_c.dir.y, sizeof(faces[j].norm_c.dir.y));
            file.read((char*)&faces[j].norm_c.dir.z, sizeof(faces[j].norm_c.dir.z));
            file.read((char*)&faces[j].text_a.x, sizeof(faces[j].text_a.x));
            file.read((char*)&faces[j].text_a.y, sizeof(faces[j].text_a.y));
            file.read((char*)&faces[j].text_a.z, sizeof(faces[j].text_a.z));
            file.read((char*)&faces[j].text_b.x, sizeof(faces[j].text_b.x));
            file.read((char*)&faces[j].text_b.y, sizeof(faces[j].text_b.y));
            file.read((char*)&faces[j].text_b.z, sizeof(faces[j].text_b.z));
            file.read((char*)&faces[j].text_c.x, sizeof(faces[j].text_c.x));
            file.read((char*)&faces[j].text_c.y, sizeof(faces[j].text_c.y));
            file.read((char*)&faces[j].text_c.z, sizeof(faces[j].text_c.z));
        }

        meshes[i].init(vrtcs, faces, faces_cnt);
        delete[] vrtcs;
        delete[] faces;
    }

    int nt = -1;
    file.read((char*)&nt, sizeof(nt));
    if (nt < 0) {
        DFLOG.addf("Value: incorrect number of inbuild textures (%d)\n\tfile:  %s\n", nt, file_path);
        return;
    }
    if (nt == 0) {
        DFLOG.addf("Warning: there are no inbuild textures \n\tfile:  %s\n", file_path);
    }

    TEXTURE* texture = new TEXTURE[nt];
    for (int i = 0; i < nt; ++i) {
        int name_len;
        char* name;
        file.read((char*)&name_len, sizeof(name_len));
        if (name_len <= 0) {
            DFLOG.addf("Value: incorrect texture name length (%d)\n\tfile: %s\n", name_len, file_path);
            return;
        }
        name = new char[name_len + 1];
        name[name_len] = '\0';
        file.read(name, name_len);

        int x = 0;
        int y = 0;
        file.read((char*)&x, sizeof(x));
        file.read((char*)&y, sizeof(y));
        if (x <= 1 || y <= 1) {
            DFLOG.addf("Value: incorrect size of texture %s (X = %d, Y = %d)\n\tfile: %s\n", name, x, y, file_path);
            return;
        }

        char* data = new char[3 * x * y];
        file.read((char*)data, 3 * x * y);
        texture[i].init((unsigned char*)data, x, y, name);
        delete[] data;
        delete[] name;
    }

    for (int i = 0; i < materials_cnt; ++i) {
        materials[i].texture.init((char*)mtn[i].c_str());
    }

    delete[] texture;
    delete[] mtn;
    file.close();
}

P3D MODEL_STAT::get_collision(const P3D& begin, const P3D& end, V3D* normal)
{
    V3D r(begin, end);
    P3D result = end;

    // TODO
    // V3D n(0, 0, 0);
    // TRIANGLE tria;
    // float length = r.get_length();

    // for (int i = 0; i < meshes_cnt; ++i) {
    //     int nf = meshes[i].arrayDim * 3;

    //     for (int j = 0; j < nf; ++j) {
    //         tria.idx_a.x = meshes[i].vertex[j++];
    //         tria.idx_a.y = meshes[i].vertex[j++];
    //         tria.idx_a.z = meshes[i].vertex[j++];
    //         tria.idx_b.x = meshes[i].vertex[j++];
    //         tria.idx_b.y = meshes[i].vertex[j++];
    //         tria.idx_b.z = meshes[i].vertex[j++];
    //         tria.idx_c.x = meshes[i].vertex[j++];
    //         tria.idx_c.y = meshes[i].vertex[j++];
    //         tria.idx_c.z = meshes[i].vertex[j];
    //         if (!tria.idx_build())
    //             continue;
    //         else
    //         {
    //             P3D c = tria.idx_check(begin, end);
    //             V3D v(begin, c);
    //             float l = v.GetLength();
    //             if (length <= l)    //there is nearer point
    //                 continue;
    //             length = l;
    //             result = c;
    //             n = tria.normal;
    //         }
    //     }
    // }
    // if (normal)
    //     *normal = n;

    return result;
}

void MODEL_STAT::get_boundaries(math::P3D* min_xyz, math::P3D* max_xyz) const
{
    P3D min, max;

    if (!min_xyz || !max_xyz) {
        return;
    }

    if (!meshes || !meshes_cnt) {
        *min_xyz = min;
        *max_xyz = max;
        return;
    }

    meshes[0].get_boundaries(&min, &max);

    for (int i = 1; i < meshes_cnt; ++i) {
        P3D n, x;

        meshes[i].get_boundaries(&n, &x);
        if (n.x < min.x)
            min.x = n.x;
        if (n.y < min.y)
            min.y = n.y;
        if (n.z < min.z)
            min.z = n.z;
        if (x.x > max.x)
            max.x = x.x;
        if (x.y > max.y)
            max.y = x.y;
        if (x.z > max.z)
            max.z = x.z;
    }

    *min_xyz = min;
    *max_xyz = max;
}
