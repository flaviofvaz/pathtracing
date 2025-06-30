#ifndef SCENE_H
#define SCENE_H

#include "ray.h"
#include "hit.h"
#include "light.h"
#include "instance.h"
#include <memory>
#include <vector>

class Scene
{
    private:
        std::vector<std::unique_ptr<Instance>> sceneObjects;
        std::vector<Instance*> lightInstances;
        glm::vec3 ambientLight;
    public:
        Scene() = default;
        ~Scene() = default;

        // Prevent copying
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        // Allow moving
        Scene(Scene&&) = default;
        Scene& operator=(Scene&&) = default;

        Light* SampleLight(float* lpdf) const;
        std::unique_ptr<Hit> computeIntersection(const Ray& ray) const;
        const glm::vec3 tracePath(Ray& ray, const int dMax) const;

        void addObject(std::unique_ptr<Instance> sceneObject) 
        {
            if (sceneObject->isLight()) 
            {
                lightInstances.push_back(sceneObject.get());
            }
            sceneObjects.push_back(std::move(sceneObject));
        }

        const std::vector<std::unique_ptr<Instance>>& getObjects() const { return sceneObjects; }
        const glm::vec3& getAmbientLight() const { return ambientLight; }
        void setAmbientLight(const glm::vec3& light) { ambientLight = light; }
        const glm::vec3 GetLightRadiance(const glm::vec3 p, const glm::vec3 n) const;
        const glm::vec3 HemisphereToGlobal(glm::vec3 p, glm::vec3 n, glm::vec3 wih) const;
};
#endif