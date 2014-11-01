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

#ifndef __MODEL_STAT_HPP__
#define __MODEL_STAT_HPP__

#include "model.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace model_data
{

class MESH_M : public MESH
{
public:
    int m_idx;                              // external index of used material
};

} // namespace model_data

namespace model
{

class MODEL_STAT : public MODEL
{
protected:
    model_data::MATERIAL* materials;        // list of used materials
    model_data::MESH_M* meshes;             // all meshes of the model
    int materials_cnt;
    int meshes_cnt;

public:
    math::P3D position;                     // origin
    math::P3D rotation;                     // pitch roll yaw
    math::P3D scale;                        // scale the model

public:
    MODEL_STAT();
    ~MODEL_STAT();

    MODEL_STAT(const MODEL_STAT&);
    MODEL_STAT& operator =(const MODEL_STAT&);

    virtual void render(float time = 0) const;
    virtual void update(float time = 0);

    void init(const char* file_path);
    math::P3D get_colision_point(const math::P3D& begin, const math::P3D& end, math::V3D* normal = NULL);
    void get_boundaries(math::P3D* min_xyz, math::P3D* max_xyz) const;
    void to_sub_model(const math::P3D& centre, float half_edge);
    int enum_polygons() const;
};

} // namespace model

#endif // __MODEL_STAT_HPP__
