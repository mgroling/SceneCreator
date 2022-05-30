#include "Eigen/Dense"

using Eigen::Vector3d;

class Canvas
{
public:
    Vector3d origin;
    Vector3d vec1;
    Vector3d vec2;
    Canvas(Vector3d pov, Vector3d look_point, double canvas_distance,
           double img_width, double img_height, double canvas_width,
           double canvas_height)
    {
        // create a plane that is perpendicular to the view vector and use it as canvas
        Vector3d u = look_point - pov;
        this->vec1 = Vector3d(u[1], -u[0], 0).normalized() * canvas_width / img_width;
        this->vec2 = u.cross(this->vec1).normalized() * canvas_height / img_height;

        // canvas should be canvas_distance away from pov
        this->origin = pov + u.normalized() * canvas_distance;
        // move origin to top left of canvas
        this->origin = this->origin - this->vec1 * img_width / 2 - this->vec2 * img_height / 2;
    }

    Vector3d call(double u, double v) const
    {
        return this->origin + this->vec1 * u + this->vec2 * v;
    }
};
