#ifndef Hittable_H
#define Hittable_H

#include "Eigen/Dense"
#include "Ray.h"

using Eigen::Vector3d;

class HitRecord
{
public:
    Vector3d baryCoords; // the first two are the barycentric coordinates and the last one is the t (of the ray that hit it)
    Vector3d normal;     // the surface normal
    Vector3d point;      // the point that was hit
    bool ignoreLight;    // if this object should be ignored for lighting purposes (its the source itself or is transparent)

    HitRecord() {}
};

class Hittable
{
public:
    virtual bool hit(const Ray &ray, HitRecord &hit) const = 0;

    virtual std::array<uint8_t, 3> color(const Ray &ray, const HitRecord &hit) const = 0;
};

#endif