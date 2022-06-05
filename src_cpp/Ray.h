#include "Eigen/Dense"

using Eigen::Vector3d;

class Ray
{
public:
    Vector3d origin;
    Vector3d direction;
    Ray(Vector3d ray_origin, Vector3d ray_direction)
    {
        origin = ray_origin;
        direction = ray_direction.normalized();
    }

    Vector3d call(double t) const { return origin + direction * t; }
};