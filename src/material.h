#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

// Forward declarations
class Scene;
class Hit;

class Material
{
    public:
        Material() = default;
        virtual ~Material() = default;
        virtual glm::vec3 GetSample(float* pdf) const = 0;
        virtual glm::vec3 GetBRDF() const = 0;
};

class PhongMaterial : public Material
{
    private:
        glm::vec3 diffuse;

    public:
        PhongMaterial(const glm::vec3& diffuse)
            : diffuse(diffuse) {}
        
        glm::vec3 GetSample(float* pdf) const override;
        glm::vec3 GetBRDF() const override;

};
#endif