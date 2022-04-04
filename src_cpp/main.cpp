#include "Dice.h"
#include <iostream>

using namespace std;

int main()
{
    Vector3 u = Vector3(0, 0, 0);
    Vector3 v = (Vector3(1, 0, 0) - u).normalized();
    Vector3 w = (Vector3(0, 1, 0) - u).normalized();
    D6Dice dice = D6Dice(u, 2);
    Ray r = Ray(Vector3(0, 0, 10), Vector3(0, 0, -1).normalized());
    HitRecord hit;
    dice.hit(r, hit);
    std::cout << "result: " << hit.baryCoords << "\n";
    return 0;
}