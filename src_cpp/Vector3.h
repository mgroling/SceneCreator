#ifndef Vector_H
#define Vector_H
#include <iostream>
#include <math.h>
using namespace std;

class Vector3 {
public:
    float x;
    float y;
    float z;
    Vector3() { }
    Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // basic functions
    Vector3 operator-() { return Vector3(-this->x, -this->y, -this->z); }
    Vector3 operator+(Vector3 other)
    {
        return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
    }
    Vector3 operator-(Vector3 other)
    {
        return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
    }
    Vector3 operator*(Vector3 other)
    {
        return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
    }
    Vector3 operator/(Vector3 other)
    {
        return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
    }
    Vector3 operator+(float other)
    {
        return Vector3(this->x + other, this->y + other, this->z + other);
    }
    Vector3 operator-(float other)
    {
        return Vector3(this->x - other, this->y - other, this->z - other);
    }
    Vector3 operator*(float other)
    {
        return Vector3(this->x * other, this->y * other, this->z * other);
    }
    Vector3 operator/(float other)
    {
        return Vector3(this->x / other, this->y / other, this->z / other);
    }

    // typical vector functions
    float norm() { return sqrt(this->x * this->x + this->y * this->y + this->z * this->z); }
    Vector3 normalized()
    {
        float temp = this->norm();
        return Vector3(this->x / temp, this->y / temp, this->z / temp);
    }
    float dot(Vector3 other) { return this->x * other.x + this->y * other.y + this->z * other.z; }
    Vector3 cross(Vector3 other)
    {
        return Vector3(this->y * other.z - other.y * this->z,
            -(this->x * other.z - other.x * this->z), this->x * other.y - other.x * this->y);
    }
};

std::ostream& operator<<(std::ostream& out, Vector3 data)
{
    out << data.x << " " << data.y << " " << data.z;
    return out;
}

#endif