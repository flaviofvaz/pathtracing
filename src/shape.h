#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include "ray.h"
#include "hit.h"

class Shape
{
    public:
        virtual ~Shape() = default;
        virtual bool intersect(const Ray& ray, Hit* hit) const = 0;
};

class Sphere : public Shape
{
    private:
        glm::vec3 center;
        float radius;
    public:
        Sphere(const glm::vec3& center, float radius);
        bool intersect(const Ray& ray, Hit* hit) const override;
};

class Box : public Shape
{
    private:
        glm::vec3 bMin;
        glm::vec3 bMax;
    public:
        Box(const glm::vec3& bMin, const glm::vec3& bMax);
        bool intersect(const Ray& ray, Hit* hit) const override;
};
#endif