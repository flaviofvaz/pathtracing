#include "material.h"
#include <glm/glm.hpp>
#include "scene.h"
#include "light.h"

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