#include "Vector3.h"
#include <math.h>

class Matrix3 {
public:
    Vector3 a;
    Vector3 b;
    Vector3 c;
    Matrix3(Vector3 vec1, Vector3 vec2, Vector3 vec3)
    {
        // Vectors are columns of the matrix
        this->a = vec1;
        this->b = vec2;
        this->c = vec3;
    }

    // basic functions
    Matrix3 operator+(float other)
    {
        return Matrix3(this->a + other, this->b + other, this->c + other);
    }
    Matrix3 operator-(float other)
    {
        return Matrix3(this->a - other, this->b - other, this->c - other);
    }
    Matrix3 operator*(float other)
    {
        return Matrix3(this->a * other, this->b * other, this->c * other);
    }
    Matrix3 operator/(float other)
    {
        return Matrix3(this->a / other, this->b / other, this->c / other);
    }

    // typical matrix functions
    float determinant()
    {
        return this->a.x * (this->b.y * this->c.z - (this->b.z * this->c.y))
            - this->b.x * (this->a.y * this->c.z - (this->a.z * this->c.y))
            + this->c.x * (this->a.y * this->b.y - (this->a.z * this->b.z));
    }
    Matrix3 transpose()
    {
        return Matrix3(Vector3(this->a.x, this->b.x, this->c.x),
            Vector3(this->a.y, this->b.y, this->c.y), Vector3(this->a.z, this->b.z, this->c.z));
    }
    Matrix3 inverse() { return this->transpose().matrixOfMinors() / this->determinant(); }
    Vector3 matmul(Vector3 other)
    {
        return Vector3(this->a.x * other.x + this->b.x * other.y + this->c.x * other.z,
            this->a.y * other.x + this->b.y * other.y + this->c.y * other.z,
            this->a.z * other.x + this->b.z * other.y + this->c.z * other.z);
    }
    Matrix3 matrixOfMinors()
    {
        return Matrix3(Vector3(this->b.y * this->c.z - (this->b.z * this->c.y),
                           -(this->b.x * this->c.z - (this->b.z * this->c.x)),
                           this->b.x * this->c.y - (this->b.y * this->c.x)),
            Vector3(-(this->a.y * this->c.z - (this->a.z * this->c.y)),
                this->a.x * this->c.z - (this->a.z * this->c.x),
                -(this->a.x * this->c.y - (this->a.y - this->c.x))),
            Vector3(this->a.y * this->b.z - (this->a.z * this->b.y),
                -(this->a.x * this->b.z - (this->a.z * this->b.x)),
                this->a.x * this->b.y - (this->a.y * this->b.x)));
    }
};

std::ostream& operator<<(std::ostream& out, Matrix3 data)
{
    out << data.a.x << " " << data.b.x << " " << data.c.x << "\n"
        << data.a.y << " " << data.b.y << " " << data.c.y << "\n"
        << data.a.z << " " << data.b.z << " " << data.c.z;
    return out;
}