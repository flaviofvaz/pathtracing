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

glm::vec3 Scene::traceRay(const Ray& ray) const
{
    auto hit = computeIntersection(ray);
    if (!hit) 
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (hit->isLight()) 
    {
        const Light* light = hit->getLight();
        auto r = hit->t;
        glm::vec3 color = getAmbientLight() + light->getPower() / (r * r);
        return color;
    } 
    else 
    {
        glm::vec3 materialColor = hit->getMaterial()->Eval(this, hit.get(), ray.getRayOrigin());
        return materialColor;
    }
}
