#include "Eigen/Dense"
#include "Surface.h"
#include <math.h>

using Eigen::Vector3d;

// creates a six-sided dice with side-length size and center as its center
// all surfaces are then pushed to world
void createD6Dice(Vector3d center, double size, Vector3d rotation, const std::array<const Texture *, 6> &textures,
                  const std::array<const map_func, 6> &mapping_functions, std::vector<const Hittable *> &world)
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
    Eigen::AngleAxisd rotX = Eigen::AngleAxisd(rotation[0], Vector3d(1, 0, 0));
    Eigen::AngleAxisd rotY = Eigen::AngleAxisd(rotation[1], Vector3d(0, 1, 0));
    Eigen::AngleAxisd rotZ = Eigen::AngleAxisd(rotation[2], Vector3d(0, 0, 1));
    for (int i = 0; i < 8; i++)
    {
        vertices[i] = center + rotX * (vertices[i] - center);
        vertices[i] = center + rotY * (vertices[i] - center);
        vertices[i] = center + rotZ * (vertices[i] - center);
    }

    int surface_groups[6][3] = {{7, 6, 5}, {0, 1, 4}, {1, 3, 5}, {3, 2, 7}, {2, 0, 6}, {3, 2, 1}};
    for (int i = 0; i < 6; i++)
    {
        world.push_back(new RectangularSurface(vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][1]] - vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][2]] - vertices[surface_groups[i][0]],
                                               textures[i], mapping_functions[i]));
    }
};