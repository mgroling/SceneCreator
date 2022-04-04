#include "Surface.h"
#include <math.h>

#include <iostream>

using namespace std;

class RectangularSurface : public Hittable {
public:
    Surface surf;
    RectangularSurface() { }
    RectangularSurface(Vector3 origin, Vector3 vec1, Vector3 vec2)
    {
        this->surf = Surface(origin, vec1, vec2);
    }

    bool hit(Ray ray, HitRecord& hit) override
    {
        return this->surf.getBarycentricCoordinates(ray, hit)
            && !(hit.baryCoords.x < 0 || hit.baryCoords.x > 1 || hit.baryCoords.y < 0
                || hit.baryCoords.y > 1);
    }
};

class D6Dice : public Hittable {
public:
    RectangularSurface surfs[6];
    D6Dice() { }
    D6Dice(Vector3 center, float size)
    {
        Vector3 vertices[8] = {
            Vector3(center.x - size / 2, center.y - size / 2, center.z - size / 2),
            Vector3(center.x + size / 2, center.y - size / 2, center.z - size / 2),
            Vector3(center.x - size / 2, center.y + size / 2, center.z - size / 2),
            Vector3(center.x + size / 2, center.y + size / 2, center.z - size / 2),
            Vector3(center.x - size / 2, center.y - size / 2, center.z + size / 2),
            Vector3(center.x + size / 2, center.y - size / 2, center.z + size / 2),
            Vector3(center.x - size / 2, center.y + size / 2, center.z + size / 2),
            Vector3(center.x + size / 2, center.y + size / 2, center.z + size / 2),
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
            if (this->surfs[i].hit(ray, temp) & temp.baryCoords.z < min_t) {
                min_t = temp.baryCoords.z;
                hit = temp;
                any_hit = true;
            }
        }
        return any_hit;
    }
};