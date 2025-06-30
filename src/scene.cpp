#include "scene.h"
#include "hit.h"
#include "light.h"


#define EPSILON 1e-4f

std::unique_ptr<Hit> Scene::computeIntersection(const Ray& ray) const
{
    std::unique_ptr<Hit> closestHit = nullptr;
    float min_t = std::numeric_limits<float>::infinity();

    for (const auto& instance : sceneObjects)
    {
        auto currentHit = instance->computeIntersection(ray);
        if (currentHit && currentHit->t < min_t)
        {
            min_t = currentHit->t;
            closestHit = std::move(currentHit);
        }
    }
    return closestHit;
}

const glm::vec3 Scene::HemisphereToGlobal(glm::vec3 p, glm::vec3 n, glm::vec3 wih) const
{
    glm::vec3 t = glm::vec3(1.0f, 0.0f, 0.0f);

    if(glm::dot(t, n) > 0.9f)
    {
        t = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 b = glm::normalize(glm::cross(n, t));
    t = glm::cross(b, n);

    glm::mat3 M = glm::mat3(t, b, n);

    return glm::normalize(M * wih);
}

Light* Scene::SampleLight(float* lpdf) const
{   
    if (lightInstances.empty()) {
        *lpdf = 0.0f;
        return nullptr;
    }
    
    // calculate total power of all lights
    float totalPower = 0.0f;
    std::vector<float> lightPowers;
    
    for (const auto& instance : lightInstances) {
        Light* light = instance->getLight();
        glm::vec3 power = light->getPower();
        float lightPower = power.x + power.y + power.z;
        lightPowers.push_back(lightPower);
        totalPower += lightPower;
    }
    
    if (totalPower == 0.0f) {
        *lpdf = 0.0f;
        return nullptr;
    }
    
    // sample based on power distribution
    float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * totalPower;
    float cumulativePower = 0.0f;
    
    for (size_t i = 0; i < lightInstances.size(); ++i) {
        cumulativePower += lightPowers[i];
        if (randomValue <= cumulativePower) {
            // set lpdf = Pi / ∑Pj (power of selected light / total power)
            *lpdf = lightPowers[i] / totalPower;
            return lightInstances[i]->getLight();
        }
    }
    
    // fallback
    return nullptr;
}

const glm::vec3 Scene::GetLightRadiance(const glm::vec3 p, const glm::vec3 n) const 
{   float lpdf = 0.0f;
    float pdf = 0.0f;
    glm::vec3 ns;
    Light* light = this->SampleLight(&lpdf);
    glm::vec3 s = light->getSample(&pdf, ns);

    glm::vec3 dif = s - p;
    float distance = glm::length(dif);
    glm::vec3 wi = dif / distance;

    Ray ray = Ray(p + EPSILON * n, wi);
    auto hit = this->computeIntersection(ray);
    if (!hit || !hit->isLight())
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        float d = distance * distance;
        auto I = light->GetIrradiance();
        return (I * glm::max(0.0f, glm::dot(n, wi)) * glm::max(0.0f, glm::dot(ns, -wi))) / (d * lpdf * pdf);
    }
}

const glm::vec3 Scene::tracePath(Ray& ray, const int dMax) const
{
    glm::vec3 L = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 beta = glm::vec3(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < dMax; i++)
    {
        auto hit = computeIntersection(ray);
        if (!hit) 
        {
            break;
        }

        if (hit->isLight()) 
        {
            if (i == 0)
            {
                const Light* light = hit->getLight();
                L += beta * light->GetIrradiance();
            }
            break;
        } 
        else 
        {
            glm::vec3 p = hit->position;
            glm::vec3 n = hit->normal;

            glm::vec3 Le = this->GetLightRadiance(p, n);
            L += Le * hit->getMaterial()->GetBRDF() * beta;

            float pdf;
            glm::vec3 wih = hit->getMaterial()->GetSample(&pdf);
            glm::vec3 wi = this->HemisphereToGlobal(p, n, wih);
            
            beta *= hit->getMaterial()->GetBRDF() * glm::max(0.0f, glm::dot(n, wi)) / pdf;
            ray = Ray(p + EPSILON * n, wi);
        }
    }

    return L;
}
