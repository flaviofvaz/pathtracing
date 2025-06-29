#ifndef HIT_H
#define HIT_H

#include <glm/glm.hpp>
#include "light.h"
#include "material.h"

class Hit
{
    private:
        enum class InstanceType { NONE, LIGHT, MATERIAL };
        union 
        {
            const Light* light;
            const Material* material;
        };
        InstanceType type;
    public:
        Hit();
        Hit(float t, const glm::vec3& position, const glm::vec3& normal, bool backface);
        Hit(const Hit& other);
        Hit& operator=(const Hit& other);
        ~Hit();

        bool backface;
        float t;
        glm::vec3 position;
        glm::vec3 normal;

        bool isLight() const { return type == InstanceType::LIGHT; }
        bool isMaterial() const { return type == InstanceType::MATERIAL; }
        const Light* getLight() const { return type == InstanceType::LIGHT ? light : nullptr; }
        const Material* getMaterial() const { return type == InstanceType::MATERIAL ? material : nullptr; }
        void setMaterial(const Material* material);
        void setLight(const Light* light);
};
#endif