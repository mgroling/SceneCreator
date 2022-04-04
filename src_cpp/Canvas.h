#include "Vector3.h"

class Canvas {
public:
    Vector3 origin;
    Vector3 vec1;
    Vector3 vec2;
    Canvas(Vector3 pov, Vector3 look_point, float canvas_distance, float width, float height)
    {
        // create a plane that is perpendicular to the view vector and use it as canvas
        Vector3 u = look_point - pov;
        this->vec1 = Vector3(u.y, -u.x, 0).normalized();
        this->vec2 = u.cross(this->vec1).normalized();

        // canvas should be canvas_distance away from pov
        this->origin = pov + u.normalized() * canvas_distance;
        // move origin to top left of canvas
        this->origin = this->origin - this->vec1 * width / 2 - this->vec2 * height / 2;
    }

    Vector3 call(float u, float v) { return this->origin + this->vec1 * u + this->vec2 * v; }
};
