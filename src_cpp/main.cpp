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

// work function for threads that executes raycasting
// (all hittable objects are checked and the one hit with minimal distance is asked for a color value), if none are hit the background color is assigned
template <int img_width, int img_height>
void work(std::array<std::array<std::array<uint8_t, 3>, img_width>, img_height> &img, const std::vector<const Hittable *> &world,
          const std::vector<const AbstractLight *> lights, const Vector3d &pov, const Canvas &canvas, const int thread_id, const int &num_threads)
{
    for (int y = 0; y < img_height; y++)
    {
        for (int x = thread_id; x < img_width; x += num_threads)
        {
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
                    color = lights[i]->lighting(hit, normalized_temp_color, world);
                }
                img[y][x] = std::array<uint8_t, 3>{(uint8_t)(color[0] * 255), (uint8_t)(color[1] * 255), (uint8_t)(color[2] * 255)};
            }
            else
            {
                img[y][x][0] = 100;
                img[y][x][1] = 100;
                img[y][x][2] = 100;
            }
        }
    }
}

int main()
{
    // to compile:  g++ -I eigen-3.4.0 -pthread src_cpp/main.cpp -o main
    // to run:      ./main
    Timer timer;
    const int img_width = 1920;
    const int img_height = 1080;
    const int canvas_width = 192;
    const int canvas_height = 108;
    const int canvas_distance = 400;
    const int num_threads = 16; // for maximum performance this should be equal to your number of cpus
    std::array<std::array<std::array<uint8_t, 3>, img_width>, img_height> img;

    const Vector3d pov = Vector3d(400, 400, 200);
    const Vector3d look_point = Vector3d(0, 0, 0);
    const Canvas canvas = Canvas(pov, look_point, canvas_distance, img_width, img_height, canvas_width, canvas_height);
    PointLight plight = PointLight(Vector3d(50, 50, 50), Vector3d(1, 1, 1));
    std::vector<const AbstractLight *> lights = {&plight};
    std::vector<const Hittable *> world;
    world.reserve(6);
    Texture tex = Texture("textures/img.ppm");
    Texture wood_tex = Texture("textures/wood_texture.ppm");
    const map_func temp_map = [](double u, double v)
    { return std::array<double, 2>{u, v}; };
    createRectangularPrism(Vector3d(0, 0, 0), Vector3d(50, 50, 50), Vector3d(0, 0, 0),
                           std::array<const Texture *, 6>{&tex, &tex, &tex, &tex, &tex, &tex},
                           std::array<const map_func, 6>{temp_map, temp_map, temp_map, temp_map, temp_map, temp_map}, world);
    createRectangularPrism(Vector3d(100, 0, 0), Vector3d(75, 75, 75), Vector3d(0, 0, M_PI / 4),
                           std::array<const Texture *, 6>{&wood_tex, &wood_tex, &wood_tex, &wood_tex, &wood_tex, &wood_tex},
                           std::array<const map_func, 6>{temp_map, temp_map, temp_map, temp_map, temp_map, temp_map}, world);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work<img_width, img_height>, std::ref(img), std::ref(world), std::ref(lights),
                             std::ref(pov), std::ref(canvas), i, std::ref(num_threads));
    }

    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }

    std::ofstream my_file;
    my_file.open("pic.ppm");
    my_file << "P3\n"
            << img_width << " " << img_height << "\n255\n";

    for (int y = 0; y < img_height; y++)
    {
        for (int x = 0; x < img_width; x++)
        {
            my_file << +img[y][x][0] << " " << +img[y][x][1] << " " << +img[y][x][2] << "\n";
        }
    }
    my_file.close();
    return 0;
}