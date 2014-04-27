#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "vector.hpp"

namespace renderer
{

namespace camera
{

/* rules with modelview permutations */
class MCAMERA
{
protected:
    math::P3D position;
    math::V3D i;
    math::V3D j;
    math::V3D k;

    float pitch;
    float roll;
    float yaw;

    void set_camera(const math::P3D& position, const math::P3D& target);
    void set_camera(const math::P3D& position, const math::V3D& dir);
    void set_vectors(const math::V3D& i, const math::V3D& j, const math::V3D& k);
    void set_angles(const float pitch, const float roll, const float yaw);

public:
    MCAMERA();
    MCAMERA(const math::P3D& position);
    MCAMERA(const math::P3D& position, const math::P3D& target);
    MCAMERA(const math::P3D& position, const math::V3D& dir);

    virtual void apply();
    virtual void apply(const math::P3D& position, const math::P3D& target);
    virtual void apply(const math::P3D& position, const math::V3D& dir);
    virtual void apply(const math::V3D& i, const math::V3D& j, const math::V3D& k);
    virtual void apply(const float& pitch, const float& roll, const float& yaw);
};

} //namespace camera

/* rules with projection permutations (perspective) */
class PCAMERA : public camera::MCAMERA
{
protected:
    float fovy;
    float aspect;
    float z_near;
    float z_far;

public:
    PCAMERA();
    PCAMERA(float fovy, float aspect, float near, float far);
    PCAMERA(const math::P3D& position, const math::P3D& target, float fovy, float aspect, float near, float far);
    PCAMERA(const math::P3D& position, const math::V3D& dir, float fovy, float aspect, float near, float far);

    void set_camera(float fovy);
    void set_camera(float fovy, float aspect);
    void set_camera(float fovy, float aspect, float near, float far);

    void apply();
    void apply(const math::P3D& position, const math::P3D& target);
    void apply(const math::P3D& position, const math::V3D& dir);
};

/* rules with projection permutations (ortho) */
class OCAMERA : public camera::MCAMERA
{
protected:
    float left;
    float right;
    float top;
    float bottom;
    float z_near;
    float z_far;

public:
    OCAMERA();
    OCAMERA(float left, float right, float top, float bottom, float near, float far);
    OCAMERA(const math::P3D& position, const math::P3D& target, float left, float right, float top, float bottom, float near, float far);
    OCAMERA(const math::P3D& position, const math::V3D& dir, float left, float right, float top, float bottom, float near, float far);

    void set_camera(float left, float right, float top, float bottom);
    void set_camera(float left, float right, float top, float bottom, float near, float far);

    void apply();
    void apply(const math::P3D& position, const math::P3D& target);
    void apply(const math::P3D& position, const math::V3D& dir);
};

} // namespace renderer

#endif  // __CAMERA_HPP__
