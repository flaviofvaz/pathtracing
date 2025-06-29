#include "material.h"
#include <glm/glm.hpp>
#include "scene.h"
#include "light.h"

glm::vec3 PhongMaterial::Eval(const Scene* scene, const Hit* hit, const glm::vec3& rayOrigin) const
{
    glm::vec3 c = ambient * scene->getAmbientLight();
    glm::vec3 v = glm::normalize(rayOrigin - hit->position);

    for (const auto& sceneObject : scene->getObjects())
    {
        if (sceneObject->isLight())
        {
            for (int i = 0; i < sceneObject->getLight()->getSampleCount(); i++)
            {
                glm::vec3 L;
                glm::vec3 l = sceneObject->getLight()->radiance(scene, hit->position, &L);
                
                // Diffuse component
                float diffuseFactor = glm::max(0.0f, glm::dot(hit->normal, l));
                c += diffuse * L * diffuseFactor;
            
                // Glossy component
                glm::vec3 r = reflect(-l, hit->normal);
                float glossyFactor = glm::max(0.0f, glm::dot(r, v));
                c += glossy * L * std::pow(glossyFactor, shininess);
            }
        }
    }
    
    return glm::clamp(c, 0.0f, 1.0f);
}

glm::vec3 PhongMaterial::reflect(const glm::vec3& incident, const glm::vec3& normal) const
{
    return 2.0f * glm::dot(normal, incident) * normal - incident;
}

inline float Random()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec3 PhongMaterial::GetSample(float* pdf) const
{
    float rand1 = Random();
    float rand2 = Random();
    float rand1sqrt = glm::sqrt(rand1);

    float angle = glm::two_pi<float>() * rand2;

    float x = rand1sqrt* glm::cos(angle);
    float y = rand1sqrt * glm::sin(angle);
    float z = glm::sqrt(1.0f - rand1);

    *pdf = (glm::sqrt(1.0f - rand1)) / (glm::pi<float>());
    return glm::vec3(x, y, z);
}

glm::vec3 PhongMaterial:: GetBRDF() const
{
    return diffuse / glm::pi<float>();
}