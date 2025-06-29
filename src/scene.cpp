#include "scene.h"
#include "hit.h"
#include "light.h"

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

    return glm::normalize(M * wih - p);
}

std::unique_ptr<Light> Scene::SampleLight(float* lpdf) const
{   
    *lpdf = 1.0f; 
    return 0;
}

const glm::vec3 Scene::GetLightRadiance(const glm::vec3 p, const glm::vec3 n) const 
{   float lpdf = 0.0f;
    float pdf = 0.0f;
    glm::vec3 ns;
    std::unique_ptr<Light> light = this->SampleLight(&lpdf);
    glm::vec3 s = light->getSample(&pdf, ns);

    glm::vec3 dif = s - p;
    glm::vec3 wi = glm::normalize(dif);
    Ray ray = Ray(p, wi);
    auto hit = this->computeIntersection(ray);
    if (!hit || !hit->isLight())
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        float d = glm::dot(dif, dif);

        // NEED TO IMPLEMENT
        auto I = light->GetIrraciance();

        return (I * glm::max(0.0f, glm::dot(n, wi)) * glm::max(0.0f, glm::dot(ns, -wi))) / (d * d * lpdf * pdf);
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
                return light->getPower();
            }
            else
            {
                break;
            }
        } 
        else 
        {
            glm::vec3 p = hit->position;
            glm::vec3 n = hit->normal;
            glm::vec3 Le = this->GetLightRadiance(p, n);
            float pdf;

            L += Le * hit->getMaterial()->GetBRDF() * beta;

            glm::vec3 wih = hit->getMaterial()->GetSample(&pdf);
            glm::vec3 wi = this->HemisphereToGlobal(p, n, wih);
            
            beta *= hit->getMaterial()->GetBRDF() * glm::max(0.0f, glm::dot(n, wi)) / pdf;
            ray = Ray(p, wi);
        }
    }

    return L;
}
