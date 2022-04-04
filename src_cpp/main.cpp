#include "Canvas.h"
#include "Dice.h"
#include "Eigen/Dense"

#include <fstream>
#include <iostream>

using Eigen::Matrix3d;
using Eigen::Vector3d;
using namespace std;

int main()
{
    // to compile:  g++ -I eigen-3.4.0 src_cpp/main.cpp
    // to run:      ./a.out
    int width = 400;
    int height = 400;

    Vector3d pov = Vector3d(400, 400, 400);
    Vector3d look_point = Vector3d(0, 0, 0);
    Canvas canvas = Canvas(pov, look_point, 400, width, height);
    D6Dice dice = D6Dice(Vector3d(0, 0, 0), 50);
    Hittable* world[1] = { &dice };

    ofstream my_file;
    my_file.open("pic.ppm");
    my_file << "P3\n" << width << " " << height << "\n255\n";

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Vector3d vec = canvas.call(x, y);
            Ray ray = Ray(pov, (vec - pov).normalized());
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