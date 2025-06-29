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

        std::unique_ptr<Hit> computeIntersection(const Ray& ray) const;
        glm::vec3 traceRay(const Ray& ray) const;

        void addObject(std::unique_ptr<Instance> sceneObject) {
            sceneObjects.push_back(std::move(sceneObject));
        }

        const std::vector<std::unique_ptr<Instance>>& getObjects() const { return sceneObjects; }
        const glm::vec3& getAmbientLight() const { return ambientLight; }
        void setAmbientLight(const glm::vec3& light) { ambientLight = light; }
};
#endif