#include "Eigen/Dense"
#include "Surface.h"
#include <math.h>

using Eigen::Vector3d;

class D6Dice : public Hittable
{
public:
    RectangularSurface surfs[6];
    D6Dice() {}
    D6Dice(const Vector3d center, const double size)
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

        int surface_groups[6][3] = {{7, 6, 5}, {0, 1, 4}, {7, 5, 3}, {7, 6, 3}, {0, 2, 4}, {0, 1, 2}};
        for (int i = 0; i < 6; i++)
        {
            this->surfs[i] = RectangularSurface(vertices[surface_groups[i][0]],
                                                vertices[surface_groups[i][1]] - vertices[surface_groups[i][0]],
                                                vertices[surface_groups[i][2]] - vertices[surface_groups[i][0]]);
        }
    }

    bool hit(const Ray &ray, HitRecord &hit) const override
    {
        double min_t = INFINITY;
        bool any_hit = false;
        for (int i = 0; i < 6; i++)
        {
            HitRecord temp;
            if (this->surfs[i].hit(ray, temp) && temp.baryCoords[2] < min_t)
            {
                min_t = temp.baryCoords[2];
                hit = temp;
                any_hit = true;
            }
        }
        return any_hit;
    }
};