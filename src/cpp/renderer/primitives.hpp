#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include "renderer.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "vector.hpp"

namespace renderer
{

class PRIMITIVES
{
public:
    static void draw_background(const TEXTURE& image);
    static void draw_normal(const math::P3D& position, const math::V3D& direction, const float length);
    static void draw_aim(const camera::MCAMERA& camera, const TEXTURE& crest);
    static void draw_skybox(const camera::MCAMERA& camera, const TEXTURE& up, const TEXTURE& dn, const TEXTURE& ft, const TEXTURE& bk, const TEXTURE& lf, const TEXTURE& rt);
    static void draw_cube(const math::P3D& centre, float half_edge);
    static void draw_box(const math::P3D& centre, const math::P3D& size_xyz);
};

struct MATERIAL
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shine;
    TEXTURE texture;
};

/* represents a triangle */
struct FACE
{
    int idx_a;                              // index in vertex array for A
    int idx_b;                              // index in vertex array for B
    int idx_c;                              // index in vertex array for C
    math::V3D norm_a;                       // normal for A vertex
    math::V3D norm_b;                       // normal for B vertex
    math::V3D norm_c;                       // normal for C vertex
    math::P3D tex_a;                        // texture coordinates for A vertex
    math::P3D tex_b;                        // texture coordinates for B vertex
    math::P3D tex_c;                        // texture coordinates for C vertex
};

class MESH
{
private:
    float* normal;
    float* texture;
    float* vertex;
    int normal_cnt;
    int texture_cnt;
    int vertex_cnt;
    int triangles_cnt;

public:
    FACE* faces;
    math::P3D* vertecies;
    int faces_cnt;
    int vertecies_cnt;
    int material_idx;

public:
    MESH();
    ~MESH();

    MESH(const MESH&);
    MESH& operator =(const MESH&);

    void init();
    void split(const math::P3D& centre, float half_edge);

    void render() const;
    int get_size() const;
    void get_normalized_hitbox(math::P3D* min_xyz, math::P3D* max_xyz) const;
};

} // namespace renderer

#endif // __PRIMITIVES_HPP__