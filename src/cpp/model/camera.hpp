#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "vector.hpp"

namespace model
{

class CAMERA
{
private:
    math::P3D position;
    math::V3D i;
    math::V3D j;
    math::V3D k;

    float pitch;
    float roll;
    float yaw;

public:
    CAMERA();
    CAMERA(const math::P3D& position);
    CAMERA(const math::P3D& position, const math::V3D& j);

    void set_camera(const math::P3D& position, const math::P3D& target);
    void set_vectors(const math::V3D& i, const math::V3D& j, const math::V3D& k);
    void set_angles(const float& pitch, const float& roll, const float& yaw);

    void apply();
    void apply(const math::P3D& position, const math::P3D& target);
    void apply(const math::V3D& i, const math::V3D& j, const math::V3D& k);
    void apply(const float& pitch, const float& roll, const float& yaw);
};

} // namespace model

#endif  // __CAMERA_HPP__
