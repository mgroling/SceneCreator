#ifndef Hittable_H
#define Hittable_H
#include "Eigen/Dense"
#include "Ray.h"

using Eigen::Vector3d;
using Eigen::Vector3i;

class HitRecord
{
public:
    Vector3d baryCoords;
    Vector3d normal;
    std::array<uint8_t, 3> color;

    HitRecord() {}
};

class Hittable
{
public:
    virtual bool hit(const Ray &ray, HitRecord &hit) const = 0;
};

#endif