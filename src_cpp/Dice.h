#include "Eigen/Dense"
#include "Surface.h"
#include <math.h>

using Eigen::Vector3d;

// creates a six-sided dice with side-length size and center as its center
// all surfaces are then pushed to world
void createD6Dice(Vector3d center, double size, std::vector<const Hittable *> &world)
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
    std::string tex_path = "textures/img.ppm";
    for (int i = 0; i < 6; i++)
    {
        world.push_back(new RectangularSurface(vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][1]] - vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][2]] - vertices[surface_groups[i][0]],
                                               tex_path, [](double u, double v)
                                               { return std::array<double, 2>{u, v}; }));
    }
};