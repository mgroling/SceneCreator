#ifndef Hittable_H
#define Hittable_H

#include "Eigen/Dense"
#include "Ray.h"

using Eigen::Vector3d;

class HitRecord
{
public:
    Vector3d baryCoords;
    Vector3d normal;

    HitRecord() {}
};

class Hittable
{
public:
    virtual bool hit(const Ray &ray, HitRecord &hit) const = 0;

    virtual std::array<uint8_t, 3> color(const Ray &ray, const HitRecord &hit) const = 0;
};

#endif