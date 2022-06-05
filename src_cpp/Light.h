#include "Hittable.h"

#include "vector"
#include <iostream>

class AbstractLight
{
public:
    virtual Vector3d lighting(const HitRecord &hit, Vector3d &color, const std::vector<const Hittable *> &world) const = 0;
};

class PointLight : public AbstractLight
{
public:
    Vector3d lightPos;
    Vector3d lightColor;
    PointLight(Vector3d position, Vector3d color)
    {
        lightPos = position;
        lightColor = color;
    }
    Vector3d lighting(const HitRecord &hit, Vector3d &surf_color, const std::vector<const Hittable *> &world) const override
    {
        HitRecord hit_temp;
        Ray ray = Ray(hit.point, lightPos - hit.point);
        // check if another object blocks the light source that is behind the light source
        for (const Hittable *obj : world)
        {
            // if yes, then return black color
            if (obj->hit(ray, hit_temp) && !hit_temp.ignoreLight && hit_temp.baryCoords[2] > 0.001 && hit_temp.baryCoords[2] < (hit.point - lightPos).norm())
            {
                return Vector3d(0, 0, 0);
            }
        }
        // if nothing was hit calculate the color with respect to the lighting
        // calculate dot product of the light direction and the surface normal to get the impact intensity of the light
        double nDotL = std::max((hit.point - lightPos).normalized().dot(hit.normal), (hit.point - lightPos).normalized().dot(-hit.normal));
        return surf_color.cwiseProduct(lightColor * nDotL);
    }
};

class AmbientLight : public AbstractLight
{
public:
    Vector3d lightColor;
    AmbientLight(Vector3d color)
    {
        lightColor = color;
    }
    Vector3d lighting(const HitRecord &hit, Vector3d &surf_color, const std::vector<const Hittable *> &world) const override
    {
        return surf_color.cwiseProduct(lightColor);
    }
};

class DirectionalLight : public AbstractLight
{
public:
    Vector3d negativelightDirection;
    Vector3d lightColor;
    DirectionalLight(Vector3d direction, Vector3d color)
    {
        negativelightDirection = -direction;
        lightColor = color;
    }
    Vector3d lighting(const HitRecord &hit, Vector3d &surf_color, const std::vector<const Hittable *> &world) const override
    {
        HitRecord hit_temp;
        Ray ray = Ray(hit.point, negativelightDirection);
        // check if another object blocks the light source
        for (const Hittable *obj : world)
        {
            // if yes, then return black color
            if (obj->hit(ray, hit_temp) && !hit_temp.ignoreLight && hit_temp.baryCoords[2] > 0.001)
            {
                return Vector3d(0, 0, 0);
            }
        }
        // if nothing was hit calculate the color with respect to the lighting
        // calculate dot product of the light direction and the surface normal to get the impact intensity of the light
        double nDotL = std::max((negativelightDirection).normalized().dot(hit.normal), (negativelightDirection).normalized().dot(-hit.normal));
        return surf_color.cwiseProduct(lightColor * nDotL);
    }
};