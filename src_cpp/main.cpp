#include "Canvas.h"
#include "Objects.h"
#include "Light.h"
#include "Texture.h"

#include "Eigen/Dense"
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cmath>

using Eigen::Matrix3d;
using Eigen::Vector3d;

struct Timer
{
    std::chrono::_V2::system_clock::time_point start;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        std::cout << "Program took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms to execute\n";
    }
};

uint8_t clamp(double v)
{
    if (v < 0)
    {
        return 0;
    }
    if (v > 255)
    {
        return 255;
    }
    return v;
}

// work function for threads that executes raycasting
// (all hittable objects are checked and the one hit with minimal distance is asked for a color value), if none are hit the background color is assigned
void work(uint8_t *img, const std::vector<const Hittable *> &world, const std::vector<const AbstractLight *> lights,
          const Vector3d &pov, const Canvas &canvas, const int thread_id, const int &num_threads, const int &img_width, const int &img_height)
{
    for (int y = 0; y < img_height; y++)
    {
        for (int x = thread_id; x < img_width; x += num_threads)
        {
            int index = y * img_width * 3 + x * 3;
            Ray ray = Ray(pov, (canvas.call(x, y) - pov).normalized());
            HitRecord hit;
            double min_t = INFINITY;
            const Hittable *obj_hit = NULL;
            for (int i = 0; i < world.size(); i++)
            {
                HitRecord temp_hit;
                if (world[i]->hit(ray, temp_hit) && temp_hit.baryCoords[2] < min_t)
                {
                    min_t = temp_hit.baryCoords[2];
                    hit = temp_hit;
                    obj_hit = world[i];
                }
            }

            if (obj_hit)
            {
                std::array<uint8_t, 3> temp_color = obj_hit->color(ray, hit);
                Vector3d normalized_temp_color = Vector3d((double)temp_color[0] / 255.0, (double)temp_color[1] / 255.0, (double)temp_color[2] / 255.0);
                Vector3d color = Vector3d(0, 0, 0);
                for (int i = 0; i < lights.size(); i++)
                {
                    color += lights[i]->lighting(hit, normalized_temp_color, world);
                }

                img[index] = clamp(color[0] * 255);
                img[index + 1] = clamp(color[1] * 255);
                img[index + 2] = clamp(color[2] * 255);
            }
            else
            {
                img[index] = 100;
                img[index + 1] = 100;
                img[index + 2] = 100;
            }
        }
    }
}

int main()
{
    // to compile:  g++ -I eigen-3.4.0 -pthread src_cpp/main.cpp -o main
    // to run:      ./main
    Timer timer;
    const int img_width = 1920; // 4060
    const int img_height = img_width / 1.4;
    const int canvas_width = 1000;
    const int canvas_height = canvas_width / 1.4;
    const int canvas_distance = 320;
    const int num_threads = 16; // for maximum performance this should be equal to your number of cpus
    uint8_t *img = new uint8_t[img_width * img_height * 3];

    const Vector3d pov = Vector3d(5, -50, 130);
    const Vector3d look_point = Vector3d(0, 0, 130);
    const Canvas canvas = Canvas(pov, look_point, canvas_distance, img_width, img_height, canvas_width, canvas_height);

    // define scene
    // textures
    Texture wood_tex = Texture("textures/wood.ppm");
    Texture wall_tex = Texture("textures/stone.ppm");
    Texture sky_tex = Texture("textures/night_sky.ppm");
    Texture white_tex = Texture(255, 255, 255);

    // texture mapping functions
    const map_func identity_map = [](double u, double v)
    { return std::array<double, 2>{u, v}; };

    // lights
    PointLight plight = PointLight(Vector3d(30, 5, 110), Vector3d(1, 1, 1));
    AmbientLight alight = AmbientLight(Vector3d(0.3, 0.3, 0.3));
    std::vector<const AbstractLight *> lights = {&plight, &alight};

    // objects
    std::vector<const Hittable *> world;
    world.reserve(30);

    // table
    static double remap_factor_table = 30.0;
    const map_func table_map1 = [](double u, double v)
    { return std::array<double, 2>{std::fmod(u, remap_factor_table / 120.0) * (120.0 / remap_factor_table),
                                   std::fmod(v, remap_factor_table / 30.0) * (30.0 / remap_factor_table)}; };
    const map_func table_map2 = [](double u, double v)
    { return std::array<double, 2>{std::fmod(u, remap_factor_table / 120.0) * (120.0 / remap_factor_table),
                                   std::fmod(v, remap_factor_table / 4.0) * (4.0 / remap_factor_table)}; };
    createRectangularPrism(Vector3d(0, 0, 98), Vector3d(120, 30, 4), Vector3d(0, 0, 0),
                           std::array<const Texture *, 6>{&wood_tex, &wood_tex, &wood_tex, &wood_tex, &wood_tex, &wood_tex},
                           std::array<const map_func, 6>{table_map1, table_map2, table_map2, table_map2, table_map2, table_map2}, world, false);

    // point light on table
    createRectangularPrism(Vector3d(30, 5, 110), Vector3d(5, 5, 5), Vector3d(0, 0, 0),
                           std::array<const Texture *, 6>{&white_tex, &white_tex, &white_tex, &white_tex, &white_tex, &white_tex},
                           std::array<const map_func, 6>{identity_map, identity_map, identity_map, identity_map, identity_map, identity_map}, world, true);

    // outside
    world.push_back(new RectangularSurface(Vector3d(-80, 25.1, 105), Vector3d(160, 0, 0), Vector3d(0, 0, 80), &sky_tex, identity_map, false));

    // wall
    static double remap_factor_wall = 60.0;
    const map_func wall_map1 = [](double u, double v)
    { return std::array<double, 2>{std::fmod(u, remap_factor_wall / 75.0) * (75.0 / remap_factor_wall),
                                   std::fmod(v, remap_factor_wall / 300.0) * (300.0 / remap_factor_wall)}; };
    const map_func wall_map2 = [](double u, double v)
    { return std::array<double, 2>{std::fmod(u, remap_factor_wall / 160.0) * (160.0 / remap_factor_wall),
                                   std::fmod(v, remap_factor_wall / 105.0) * (105.0 / remap_factor_wall)}; };
    // left wall
    world.push_back(new RectangularSurface(Vector3d(-155, 25, 0), Vector3d(75, 0, 0), Vector3d(0, 0, 300), &wall_tex, wall_map1, false));
    // right wall
    world.push_back(new RectangularSurface(Vector3d(80, 25, 0), Vector3d(75, 0, 0), Vector3d(0, 0, 300), &wall_tex, wall_map1, false));
    // bottom wall
    world.push_back(new RectangularSurface(Vector3d(-80, 25, 0), Vector3d(160, 0, 0), Vector3d(0, 0, 105), &wall_tex, wall_map2, false));
    // top wall
    world.push_back(new RectangularSurface(Vector3d(-80, 25, 185), Vector3d(160, 0, 0), Vector3d(0, 0, 105), &wall_tex, wall_map2, false));

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work, img, std::ref(world), std::ref(lights), std::ref(pov), std::ref(canvas),
                             i, std::ref(num_threads), std::ref(img_width), std::ref(img_height));
    }

    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }

    std::ofstream my_file;
    my_file.open("pic.ppm");
    my_file << "P3\n"
            << img_width << " " << img_height << "\n255\n";

    int index = 0;
    for (int y = 0; y < img_height; y++)
    {
        for (int x = 0; x < img_width; x++)
        {
            my_file << +img[index] << " " << +img[index + 1] << " " << +img[index + 2] << "\n";
            index += 3;
        }
    }
    my_file.close();
    return 0;
}