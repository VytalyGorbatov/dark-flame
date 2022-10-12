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

#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "vector.hpp"

namespace renderer
{

/** Triangle with vertecies, normals and texture coordinates. */
struct FACE
{
    int idx_a;
    int idx_b;
    int idx_c;
    Math::V3D norm_a;
    Math::V3D norm_b;
    Math::V3D norm_c;
    Math::P3D text_a;
    Math::P3D text_b;
    Math::P3D text_c;
};

/** Bunch of faces with a single material. */
class MESH
{
private:
    float* vertex;                          // vertecies coordniates array
    float* normal;                          // normals coordinates array
    float* texture;                         // texture coordinates array
    int v_cnt;
    int n_cnt;
    int t_cnt;
    int array_cnt;

public:
    int m_idx;                              // external index of used material

public:
    MESH();
    ~MESH();

    MESH(const MESH&);
    MESH& operator =(const MESH&);

    void init(const Math::P3D* vrtcs, const FACE* faces, int faces_cnt);
    void render() const;
    void to_sub_mesh(const Math::P3D& centre, float half_edge);
    void get_boundaries(Math::P3D* min_xyz, Math::P3D* max_xyz) const;

    int enum_polygons() const
    {
        return array_cnt / 3;
    }
};

} // namespace renderer

#endif // __MESH_HPP__
