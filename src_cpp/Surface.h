#include "Eigen/Dense"
#include "Hittable.h"
#include "Texture.h"

using Eigen::Matrix3d;
using Eigen::Vector3d;

typedef std::function<std::array<double, 2>(double, double)> map_func;

// a surface object (plane might be more fitting)
class Surface
{
public:
    Vector3d origin;
    Vector3d vec1;
    Vector3d vec2;
    Vector3d normal;
    Matrix3d a;
    Surface() {}
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

    bool getBarycentricCoordinates(const Ray &ray, HitRecord &hit) const
    {
        // plug parametric equation of the ray into the implicit equation of the surface (plane) and get t
        double t = -(this->normal.dot(ray.origin - this->origin)) / this->normal.dot(ray.direction);

        // if t is <= 0, that means the ray didn't hit the surface (only a ray with negative
        // direction of ours would have hit it)
        if (t > 0)
        {
            // get barycentric coordinates of the point of intersection, by solving linear equation
            // origin + vec1 * lambda + vec2 * mu = point of intersection
            hit.baryCoords = a * (ray.call(t) - this->origin);
            hit.baryCoords[2] = t;
            hit.normal = this->normal;
            return true;
        }
        return false;
    }
};

// plane that is constrained in size (the rectangle that is spanned by origin, vec1 and vec2)
class RectangularSurface : public Hittable
{
public:
    Surface surf;
    Texture *tex;
    map_func func;
    // origin, vec1 and vec2 are used to position the plane
    // func is used to map from barycentric coordinates to image coordinates (image loaded with texture_path) (should be in [0, 1])
    RectangularSurface(Vector3d origin, Vector3d vec1, Vector3d vec2, Texture *tex, map_func func)
    {
        this->surf = Surface(origin, vec1, vec2);
        this->tex = tex;
        this->func = func;
    }

    bool hit(const Ray &ray, HitRecord &hit) const override
    {
        return this->surf.getBarycentricCoordinates(ray, hit) &&
               (hit.baryCoords[0] >= 0 && hit.baryCoords[0] <= 1 && hit.baryCoords[1] >= 0 && hit.baryCoords[1] <= 1);
    }

    std::array<uint8_t, 3> color(const Ray &ray, const HitRecord &hit) const override
    {
        std::array<double, 2> temp = (this->func)(hit.baryCoords[0], hit.baryCoords[1]);
        return this->tex->accessElement((int)(temp[1] * tex->height), (int)(temp[0] * tex->width));
    }
};
