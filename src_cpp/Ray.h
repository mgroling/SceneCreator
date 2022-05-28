#include "Eigen/Dense"

using Eigen::Vector3d;

class Ray
{
public:
    Vector3d origin;
    Vector3d direction;
    Ray(const Vector3d ray_origin, const Vector3d ray_direction)
    {
        this->origin = ray_origin;
        this->direction = ray_direction;
    }

    Vector3d call(double t) { return this->origin + this->direction * t; }
};