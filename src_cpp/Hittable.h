#ifndef Hittable_H
#define Hittable_H
#include "Ray.h"

class HitRecord {
public:
    Vector3 baryCoords;
    Vector3 normal;
    HitRecord() { }
};

class Hittable {
public:
    virtual bool hit(Ray ray, HitRecord& hit) = 0;
};

#endif