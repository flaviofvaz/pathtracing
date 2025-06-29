#include "light.h"
#include "scene.h"
#include "ray.h"

    
inline float Random()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

AreaLight::AreaLight(const glm::vec3& position, const glm::vec3& power, const glm::vec3& ei, const glm::vec3& ej, int nSamples)
    : Light(), position(position), power(power), ei(ei), ej(ej), nSamples(nSamples)
    {
        glm::vec3 crossProduct = glm::cross(ei, ej);
        normal = glm::normalize(crossProduct);
        area = glm::length(crossProduct);
    } 

glm::vec3 AreaLight::getSample(float* pdf, glm::vec3& ns) const
{   
    ns = this->normal;
    *pdf = 1.0f / this->getArea();
    return position + ei * Random() + ej * Random();
}

glm::vec3 AreaLight::GetIrraciance() const
{
    return glm::vec3(0.0f, 0.0f, 0.0f);
}