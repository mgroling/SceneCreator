#include "Eigen/Dense"

using Eigen::Vector3d;

class Ray
{
public:
    Vector3d origin;
    Vector3d direction;
    Ray(Vector3d ray_origin, Vector3d ray_direction)
    {
        this->origin = ray_origin;
        this->direction = ray_direction;
    }

    Vector3d call(double t) const { return this->origin + this->direction * t; }
};