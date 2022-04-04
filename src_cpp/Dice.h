#include "Surface.h"
#include <math.h>

#include "Eigen/Dense"

#include <iostream>

using Eigen::Vector3d;

using namespace std;

class RectangularSurface : public Hittable {
public:
    Surface surf;
    RectangularSurface() { }
    RectangularSurface(Vector3d origin, Vector3d vec1, Vector3d vec2)
    {
        this->surf = Surface(origin, vec1, vec2);
    }

    bool hit(Ray ray, HitRecord& hit) override
    {
        return this->surf.getBarycentricCoordinates(ray, hit)
            && !(hit.baryCoords[0] < 0 || hit.baryCoords[0] > 1 || hit.baryCoords[1] < 0
                || hit.baryCoords[1] > 1);
    }
};

class D6Dice : public Hittable {
public:
    RectangularSurface surfs[6];
    D6Dice() { }
    D6Dice(Vector3d center, float size)
    {
        Vector3d vertices[8] = {
            Vector3d(center[0] - size / 2, center[1] - size / 2, center[2] - size / 2),
            Vector3d(center[0] + size / 2, center[1] - size / 2, center[2] - size / 2),
            Vector3d(center[0] - size / 2, center[1] + size / 2, center[2] - size / 2),
            Vector3d(center[0] + size / 2, center[1] + size / 2, center[2] - size / 2),
            Vector3d(center[0] - size / 2, center[1] - size / 2, center[2] + size / 2),
            Vector3d(center[0] + size / 2, center[1] - size / 2, center[2] + size / 2),
            Vector3d(center[0] - size / 2, center[1] + size / 2, center[2] + size / 2),
            Vector3d(center[0] + size / 2, center[1] + size / 2, center[2] + size / 2),
        };

        int surface_groups[6][3]
            = { { 7, 6, 5 }, { 0, 1, 4 }, { 7, 5, 3 }, { 7, 6, 3 }, { 0, 2, 4 }, { 0, 1, 2 } };
        for (int i = 0; i < 6; i++) {
            this->surfs[i] = RectangularSurface(vertices[surface_groups[i][0]],
                vertices[surface_groups[i][1]] - vertices[surface_groups[i][0]],
                vertices[surface_groups[i][2]] - vertices[surface_groups[i][0]]);
        }
    }

    bool hit(Ray ray, HitRecord& hit) override
    {
        float min_t = INFINITY;
        bool any_hit = false;
        for (int i = 0; i < 6; i++) {
            HitRecord temp;
            if (this->surfs[i].hit(ray, temp) && temp.baryCoords[2] < min_t) {
                min_t = temp.baryCoords[2];
                hit = temp;
                any_hit = true;
            }
        }
        return any_hit;
    }
};