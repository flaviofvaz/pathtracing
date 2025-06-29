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
        virtual glm::vec3 Eval(const Scene* scene, const Hit* hit, const glm::vec3& rayOrigin) const = 0;
        virtual glm::vec3 GetSample(float* pdf) const = 0;
        virtual glm::vec3 GetBRDF() const = 0;
};

class PhongMaterial : public Material
{
    private:
        glm::vec3 diffuse;
        glm::vec3 glossy;
        glm::vec3 ambient;
        float shininess;

    public:
        PhongMaterial(const glm::vec3& diffuse, const glm::vec3& glossy, 
              const glm::vec3& ambient, float shininess)
            : diffuse(diffuse), glossy(glossy), ambient(ambient), shininess(shininess) {}
        
        glm::vec3 Eval(const Scene* scene, const Hit* hit, const glm::vec3& rayOrigin) const override;
        glm::vec3 reflect(const glm::vec3& l, const glm::vec3& normal) const;
        glm::vec3 GetSample(float* pdf) const override;
        glm::vec3 GetBRDF() const override;

};
#endif