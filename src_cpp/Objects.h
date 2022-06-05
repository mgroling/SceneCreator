#include "Eigen/Dense"
#include "Surface.h"
#include <math.h>

using Eigen::Vector3d;

// creates a rectangular prism (box) with specificied size and center
// each side requires a texture and mapping function from barycentric coordinates to texture coordinates
// the sides are ordered like this: top, front, right, back, left, bottom
// all surfaces are then pushed to world
void createRectangularPrism(Vector3d center, Vector3d size, Vector3d rotation, const std::array<const Texture *, 6> &textures,
                            const std::array<const map_func, 6> &mapping_functions, std::vector<const Hittable *> &world, bool ignoreLight)
{
    Vector3d vertices[8] = {
        Vector3d(center[0] - size[0] / 2, center[1] - size[1] / 2, center[2] - size[2] / 2),
        Vector3d(center[0] + size[0] / 2, center[1] - size[1] / 2, center[2] - size[2] / 2),
        Vector3d(center[0] - size[0] / 2, center[1] + size[1] / 2, center[2] - size[2] / 2),
        Vector3d(center[0] + size[0] / 2, center[1] + size[1] / 2, center[2] - size[2] / 2),
        Vector3d(center[0] - size[0] / 2, center[1] - size[1] / 2, center[2] + size[2] / 2),
        Vector3d(center[0] + size[0] / 2, center[1] - size[1] / 2, center[2] + size[2] / 2),
        Vector3d(center[0] - size[0] / 2, center[1] + size[1] / 2, center[2] + size[2] / 2),
        Vector3d(center[0] + size[0] / 2, center[1] + size[1] / 2, center[2] + size[2] / 2),
    };
    Eigen::Quaterniond rotQuat = Eigen::AngleAxisd(rotation[0], Vector3d::UnitX()) *
                                 Eigen::AngleAxisd(rotation[1], Vector3d::UnitY()) * Eigen::AngleAxisd(rotation[2], Vector3d::UnitZ());
    for (int i = 0; i < 8; i++)
    {
        vertices[i] = center + rotQuat * (vertices[i] - center);
    }

    int surface_groups[6][3] = {{7, 6, 5}, {0, 1, 4}, {1, 3, 5}, {3, 2, 7}, {2, 0, 6}, {3, 2, 1}};
    for (int i = 0; i < 6; i++)
    {
        world.push_back(new RectangularSurface(vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][1]] - vertices[surface_groups[i][0]],
                                               vertices[surface_groups[i][2]] - vertices[surface_groups[i][0]],
                                               textures[i], mapping_functions[i], ignoreLight));
    }
};