#include "Canvas.h"
#include "Dice.h"
#include "Eigen/Dense"

#include <fstream>
#include <iostream>

using Eigen::Matrix3d;
using Eigen::Vector3d;

int main()
{
    // to compile:  g++ -I eigen-3.4.0 src_cpp/main.cpp
    // to run:      ./a.out
    int img_width = 800;
    int img_height = 600;
    int canvas_width = 160;
    int canvas_height = 120;

    Vector3d pov = Vector3d(400, 400, 0);
    Vector3d look_point = Vector3d(0, 0, 0);
    Canvas canvas
        = Canvas(pov, look_point, 400, img_width, img_height, canvas_width, canvas_height);
    D6Dice dice = D6Dice(Vector3d(0, 0, 0), 50);
    Hittable* world[] = { &dice };

    std::ofstream my_file;
    my_file.open("pic.ppm");
    my_file << "P3\n" << img_width << " " << img_height << "\n255\n";

    for (int y = img_height - 1; y >= 0; y--) {
        for (int x = 0; x < img_width; x++) {
            Ray ray = Ray(pov, (canvas.call(x, y) - pov).normalized());
            HitRecord hit;
            if (world[0]->hit(ray, hit)) {
                my_file << 255 << " " << 0 << " " << 0 << "\n";
            } else {
                my_file << 0 << " " << 255 << " " << 0 << "\n";
            }
        }
    }
    my_file.close();
    return 0;
}