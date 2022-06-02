#include "Eigen/Dense"
#include "Surface.h"
#include <math.h>

using Eigen::Vector3d;

// creates a six-sided dice with side-length size and center as its center
// all surfaces are then pushed to world
void createD6Dice(Vector3d center, double size, std::array<Texture *, 6> textures,
                  std::array<map_func, 6> mapping_functions, std::vector<const Hittable *> &world)
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

    int surface_groups[6][3] = {{4, 5, 6}, {0, 1, 4}, {1, 3, 5}, {3, 2, 7}, {2, 0, 6}, {0, 1, 2}};
    for (int i = 0; i < 6; i++)
    {
        world.push_back(new RectangularSurface(vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][1]] - vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][2]] - vertices[surface_groups[i][0]],
                                               textures[i], mapping_functions[i]));
    }
};