#include "Vector3.h"

class Ray {
public:
    Vector3 origin;
    Vector3 direction;
    Ray(Vector3 ray_origin, Vector3 ray_direction)
    {
        this->origin = ray_origin;
        this->direction = ray_direction;
    }

    Vector3 call(float t) { return this->origin + this->direction * t; }
};