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

glm::vec3 PhongMetal::Eval(const Scene* scene, const Hit* hit, const glm::vec3& rayOrigin) const
{
    static thread_local int depth = 0;
    const int MAX_DEPTH = 5;

    // Stop recursion if we've reached maximum depth
    if (depth >= MAX_DEPTH) {
        return glm::vec3(0.0f);  // Return black for maximum depth
    }

    glm::vec3 p = hit->position;
    glm::vec3 n = hit->normal;
    glm::vec3 v = glm::normalize(rayOrigin - p);

    float R = r_zero + (1.0f - r_zero) * std::pow(1.0f - glm::dot(v, n), 5.0f);
    
    glm::vec3 c = (1.0f - R) * PhongMaterial::Eval(scene, hit, rayOrigin);
    glm::vec3 r = glm::normalize(PhongMaterial::reflect(v, n));
    
    depth++;
    Ray ray(p, r);
    
    // Trace reflected ray
    c += R * scene->traceRay(ray);
    depth--;
    
    return glm::clamp(c, 0.0f, 1.0f);
}