#ifndef Hittable_H
#define Hittable_H
#include "Eigen/Dense"
#include "Ray.h"

using Eigen::Vector3d;

class HitRecord {
public:
    Vector3d baryCoords;
    Vector3d normal;
    HitRecord() { }
};

class Hittable {
public:
    virtual bool hit(Ray ray, HitRecord& hit) = 0;
};

#endif