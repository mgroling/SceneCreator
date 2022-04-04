#include "Hittable.h"
#include "Matrix.h"

#include <iostream>

using namespace std;

class Surface {
public:
    Vector3 origin;
    Vector3 vec1;
    Vector3 vec2;
    Vector3 normal;
    Surface() { }
    Surface(Vector3 origin, Vector3 vec1, Vector3 vec2)
    {
        this->origin = origin;
        this->vec1 = vec1;
        this->vec2 = vec2;
        this->normal = vec1.cross(vec2);
    }

    bool getBarycentricCoordinates(Ray ray, HitRecord& hit)
    {
        // plug parametric equation of the ray into the implicit equation of the surface (plane) and
        // get t
        float t = -(this->normal.dot(ray.origin - this->origin)) / this->normal.dot(ray.direction);

        // if t is <= 0, that means the ray didn't hit the surface (only a ray with negative
        // direction of ours would have hit it)
        if (t > 0) {
            Matrix3 a = Matrix3(this->vec1, this->vec2, -this->normal).inverse();
            Vector3 b = (ray.call(t)) - this->origin;
            // get barycentric coordinates of the point of intersection, by solving linear equation
            // origin + vec1 * lambda + vec2 * mu = point of intersection
            hit.baryCoords = a.matmul(b);
            hit.baryCoords.z = t;
            hit.normal = this->normal;
            return true;
        }
        return false;
    }
};
