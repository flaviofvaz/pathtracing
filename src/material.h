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
};

class PhongMetal : public PhongMaterial
{
    private:
        float r_zero;

    public:
        PhongMetal(const glm::vec3& diffuse, const glm::vec3& glossy, 
              const glm::vec3& ambient, float shininess, float r_zero)
            : PhongMaterial(diffuse, glossy, ambient, shininess), r_zero(r_zero) {};
        
        glm::vec3 Eval (const Scene* scene, const Hit* hit, const glm::vec3& rayOrigin) const override;
};
#endif