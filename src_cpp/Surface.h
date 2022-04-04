#include "Hittable.h"

#include <iostream>

#include "Eigen/Dense"

using Eigen::Matrix3d;
using Eigen::Vector3d;

using namespace std;

class Surface {
public:
    Vector3d origin;
    Vector3d vec1;
    Vector3d vec2;
    Vector3d normal;
    Matrix3d a;
    Surface() { }
    Surface(Vector3d origin, Vector3d vec1, Vector3d vec2)
    {
        this->origin = origin;
        this->vec1 = vec1;
        this->vec2 = vec2;
        this->normal = vec1.cross(vec2);
        a << this->vec1[0], this->vec2[0], -this->normal[0], this->vec1[1], this->vec2[1],
            -this->normal[1], this->vec1[2], this->vec2[2], -this->normal[2];
        a = a.inverse().eval();
    }

    bool getBarycentricCoordinates(Ray ray, HitRecord& hit)
    {
        // plug parametric equation of the ray into the implicit equation of the surface (plane) and
        // get t
        float t = -(this->normal.dot(ray.origin - this->origin)) / this->normal.dot(ray.direction);

        // if t is <= 0, that means the ray didn't hit the surface (only a ray with negative
        // direction of ours would have hit it)
        if (t > 0) {
            Vector3d b = (ray.call(t)) - this->origin;
            // get barycentric coordinates of the point of intersection, by solving linear equation
            // origin + vec1 * lambda + vec2 * mu = point of intersection
            hit.baryCoords = a * b;
            hit.baryCoords[2] = t;
            hit.normal = this->normal;
            return true;
        }
        return false;
    }
};
