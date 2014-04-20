#include "renderer.hpp"
#include "primitives.hpp"
#include "vector.hpp"
#include "geometry.hpp"

using namespace renderer;
using namespace math;

/* if current MATERIAL is undefined yet */
#define NIL -1

MESH::MESH()
{
    faces = NULL;
    vertecies = NULL;
    faces_cnt = 0;
    vertecies_cnt = 0;

    vertex = NULL;
    normal = NULL;
    texture = NULL;
    vertex_cnt = 0;
    normal_cnt = 0;
    texture_cnt = 0;

    triangles_cnt = 0;
    material_idx = NIL;
}

MESH::~MESH()
{
    delete[] faces;
    delete[] vertecies;
    delete[] vertex;
    delete[] normal;
    delete[] texture;
}

MESH::MESH(const MESH& m)
{
    faces_cnt = m.faces_cnt;
    vertecies_cnt = m.vertecies_cnt;
    vertex_cnt = m.vertex_cnt;
    normal_cnt = m.normal_cnt;
    texture_cnt = m.texture_cnt;
    triangles_cnt = m.triangles_cnt;
    material_idx = m.material_idx;

    faces = new FACE[faces_cnt];
    for (int i = 0; i < faces_cnt; ++i) {
        faces[i] = m.faces[i];
    }
    vertecies = new P3D[vertecies_cnt];
    for (int i = 0; i < vertecies_cnt; ++i) {
        vertecies[i] = m.vertecies[i];
    }
    vertex = new float[vertex_cnt];
    for (int i = 0; i < vertex_cnt; ++i) {
        vertex[i] = m.vertex[i];
    }
    normal = new float[normal_cnt];
    for (int i = 0; i < normal_cnt; ++i){
        normal[i] = m.normal[i];
    }
    texture = new float[texture_cnt];
    for (int i = 0; i < texture_cnt; ++i) {
        texture[i] = m.texture[i];
    }
}

MESH& MESH::operator =(const MESH& m)
{
    if (&m == this) {
        return *this;
    }

    delete[] vertex;
    delete[] normal;
    delete[] texture;
    delete[] faces;
    delete[] vertecies;

    vertecies_cnt = m.vertecies_cnt;
    faces_cnt = m.faces_cnt;
    vertex_cnt = m.vertex_cnt;
    normal_cnt = m.normal_cnt;
    texture_cnt = m.texture_cnt;
    triangles_cnt = m.triangles_cnt;
    material_idx = m.material_idx;

    faces = new FACE[faces_cnt];
    for (int i = 0; i < faces_cnt; ++i) {
        faces[i] = m.faces[i];
    }
    vertecies = new P3D[vertecies_cnt];
    for (int i = 0; i < vertecies_cnt; ++i) {
        vertecies[i] = m.vertecies[i];
    }
    vertex = new float[vertex_cnt];
    for (int i = 0; i < vertex_cnt; ++i) {
        vertex[i] = m.vertex[i];
    }
    normal = new float[normal_cnt];
    for (int i = 0; i < normal_cnt; ++i) {
        normal[i] = m.normal[i];
    }
    texture = new float[texture_cnt];
    for (int i = 0; i < texture_cnt; ++i) {
        texture[i] = m.texture[i];
    }

    return *this;
}

int MESH::get_size() const
{
    return 36 + 32 * triangles_cnt;
}

void MESH::init()
{
    if (!faces || !vertecies) {
        return;
    }

    delete[] vertex;
    delete[] normal;
    delete[] texture;
    vertex_cnt = faces_cnt * 9;
    normal_cnt = faces_cnt * 9;
    texture_cnt = faces_cnt * 6;
    vertex = new float[vertex_cnt];
    normal = new float[normal_cnt];
    texture = new float[texture_cnt];

    for (int i = 0, k = 0; i < vertex_cnt; ++k) {
        normal[i] = faces[k].norm_a.dir.x;
        vertex[i++] = vertecies[faces[k].idx_a].x;
        normal[i] = faces[k].norm_a.dir.y;
        vertex[i++] = vertecies[faces[k].idx_a].y;
        normal[i] = faces[k].norm_a.dir.z;
        vertex[i++] = vertecies[faces[k].idx_a].z;
        normal[i] = faces[k].norm_b.dir.x;
        vertex[i++] = vertecies[faces[k].idx_b].x;
        normal[i] = faces[k].norm_b.dir.y;
        vertex[i++] = vertecies[faces[k].idx_b].y;
        normal[i] = faces[k].norm_b.dir.z;
        vertex[i++] = vertecies[faces[k].idx_b].z;
        normal[i] = faces[k].norm_c.dir.x;
        vertex[i++] = vertecies[faces[k].idx_c].x;
        normal[i] = faces[k].norm_c.dir.y;
        vertex[i++] = vertecies[faces[k].idx_c].y;
        normal[i] = faces[k].norm_c.dir.z;
        vertex[i++] = vertecies[faces[k].idx_c].z;
    }

    for (int i = 0, k = 0; i < texture_cnt; ++k) {
        texture[i++] = faces[k].tex_a.x;
        texture[i++] = faces[k].tex_a.y;
        texture[i++] = faces[k].tex_b.x;
        texture[i++] = faces[k].tex_b.y;
        texture[i++] = faces[k].tex_c.x;
        texture[i++] = faces[k].tex_c.y;
    }

    triangles_cnt = faces_cnt * 3;

    delete[] faces;
    faces = NULL;
    faces_cnt = 0;

    delete[] vertecies;
    vertecies = NULL;
    vertecies_cnt = 0;
}

void MESH::render() const
{
    if (vertex && normal && texture) {
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);

        /* TODO: multitexturing
        if (CORE_ARB_MULTITEXTURE) {
            glClientActiveTextureARB(GL_TEXTURE1_ARB);
            glTexCoordPointer(2, GL_FLOAT, 0, texture);
            glClientActiveTextureARB(GL_TEXTURE0_ARB);
        }
        */
        glTexCoordPointer(2, GL_FLOAT, 0, texture);

        glDrawArrays(GL_TRIANGLES, 0, triangles_cnt);
    }
}

void MESH::get_normalized_hitbox(P3D* min_xyz, P3D* max_xyz) const
{
    if (!vertex || triangles_cnt <= 0) {
        return;
    }

    P3D min, max, a;

    a.x = vertex[0];
    a.y = vertex[1];
    a.z = vertex[2];
    min = max = a;

    for (int i = 1; i < triangles_cnt; ++i) {
        a.x = vertex[3 * i    ];
        a.y = vertex[3 * i + 1];
        a.z = vertex[3 * i + 2];

        min.x = a.x < min.x ? a.x : min.x;
        min.y = a.y < min.y ? a.y : min.y;
        min.z = a.z < min.z ? a.z : min.z;

        max.x = a.x > max.x ? a.x : max.x;
        max.y = a.y > max.y ? a.y : max.y;
        max.z = a.z > max.z ? a.z : max.z;
    }

    if (min_xyz) {
        *min_xyz = min;
    }
    if (max_xyz) {
        *max_xyz = max;
    }
}

void MESH::split(const P3D& centre, float half_edge)
{
    bool* check = new bool[triangles_cnt];
    P3D a, b, c;
    int nv = 0;

    for (int i = 0; i < triangles_cnt; ++i) {
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

        if (is_triangle_in_cube(centre, half_edge, a, b, c)) {
            check[i - 2] = check[i - 1] = check[i] = true;
            nv += 3;
        } else {
            check[i - 2] = check[i - 1] = check[i] = false;
        }
    }

    if (nv == 0) {
        delete[] vertex;
        delete[] normal;
        delete[] texture;
        vertex = NULL;
        normal = NULL;
        texture = NULL;
        vertex_cnt = 0;
        normal_cnt = 0;
        texture_cnt = 0;
        triangles_cnt = 0;

        delete[] check;
        return;
    }

    float *n, *v, *t;
    n = new float[nv * 3];
    v = new float[nv * 3];
    t = new float[nv * 2];

    for (int i = 0, j = 0; i < triangles_cnt; ++i) {
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

    triangles_cnt = nv;
    vertex_cnt = triangles_cnt * 3;
    normal_cnt = triangles_cnt * 3;
    texture_cnt = triangles_cnt * 2;

    delete check;
}
