#include "Eigen/Dense"

using Eigen::Vector3d;

// canvas (or camera) that can be used to paint/take a picture of a scenery
class Canvas
{
public:
    Vector3d origin;
    Vector3d vec1;
    Vector3d vec2;
    Canvas(const Vector3d &pov, const Vector3d &look_point, const double &canvas_distance,
           const double &img_width, const double &img_height, const double &canvas_width,
           const double &canvas_height)
    {
        // create a plane that is perpendicular to the view vector and use it as canvas
        Vector3d u = look_point - pov;
        vec1 = Vector3d(u[1], -u[0], 0).normalized() * canvas_width / img_width;
        vec2 = u.cross(vec1).normalized() * canvas_height / img_height;

        // canvas should be canvas_distance away from pov
        origin = pov + u.normalized() * canvas_distance;
        // move origin to top left of canvas
        origin = origin - vec1 * img_width / 2 - vec2 * img_height / 2;
    }

    Vector3d call(double u, double v) const
    {
        return origin + vec1 * u + vec2 * v;
    }
};
