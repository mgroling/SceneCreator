#ifndef Hittable_H
#define Hittable_H
#include "Eigen/Dense"
#include "Ray.h"

using Eigen::Vector3d;
using Eigen::Vector3i;

class HitRecord {
public:
    Vector3d baryCoords;
    Vector3d normal;
    Vector3i color;

    HitRecord() { }
};

class Hittable {
public:
    virtual bool hit(const Ray& ray, HitRecord& hit) = 0;
};

#endif