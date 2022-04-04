#include "Eigen/Dense"

using Eigen::Vector3d;

class Canvas {
public:
    Vector3d origin;
    Vector3d vec1;
    Vector3d vec2;
    Canvas(Vector3d pov, Vector3d look_point, float canvas_distance, float width, float height)
    {
        // create a plane that is perpendicular to the view vector and use it as canvas
        Vector3d u = look_point - pov;
        this->vec1 = Vector3d(u[1], -u[0], 0).normalized();
        this->vec2 = u.cross(this->vec1).normalized();

        // canvas should be canvas_distance away from pov
        this->origin = pov + u.normalized() * canvas_distance;
        // move origin to top left of canvas
        this->origin = this->origin - this->vec1 * width / 2 - this->vec2 * height / 2;
    }

    Vector3d call(float u, float v) { return this->origin + this->vec1 * u + this->vec2 * v; }
};
