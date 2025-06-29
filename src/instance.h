#ifndef INSTANCE_H
#define INSTANCE_H

#include "material.h"
#include "light.h"
#include "shape.h"
#include "hit.h"
#include "ray.h"
#include "transform.h"
#include <memory>

class Instance
{
    private:
        enum class InstanceType { NONE, LIGHT, MATERIAL };
        union 
        {
            Light* light;
            Material* material;
        };
        InstanceType type;
        std::unique_ptr<Shape> shape;
        std::unique_ptr<Transform> transform;

    public:
        Instance(std::unique_ptr<Shape> shape);
        ~Instance();

        // Prevent copying
        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;

        // Allow moving
        Instance(Instance&& other) noexcept;
        Instance& operator=(Instance&& other) noexcept;

        void setMaterial(Material* material);
        void setLight(Light* light);

        bool isLight() const { return type == InstanceType::LIGHT; }
        bool isMaterial() const { return type == InstanceType::MATERIAL; }
        Light* getLight() const { return type == InstanceType::LIGHT ? light : nullptr; }
        Material* getMaterial() const { return type == InstanceType::MATERIAL ? material : nullptr; }
        const Shape* getShape() const { return shape.get(); }
        const Transform* getTransform() const { return transform.get(); }
        void translate(const glm::vec3& translation);
        void scale(const glm::vec3& scale);
        void rotate(float angle, const glm::vec3& axis);    

        std::unique_ptr<Hit> computeIntersection(const Ray& ray) const;
};
#endif