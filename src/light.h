#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

// Forward declaration
class Scene;

class Light
{
    public:
        virtual ~Light() = default;
        virtual glm::vec3 radiance(const Scene* scene, const glm::vec3& point, glm::vec3* L) const = 0;
        virtual glm::vec3 GetIrraciance() const = 0;
        virtual glm::vec3 getPower() const = 0;
        virtual int getSampleCount() const = 0;
        virtual glm::vec3 getSample(float* pdf, glm::vec3& ns) const = 0;
};

class AreaLight : public Light
{   
    private:
        glm::vec3 position;
        glm::vec3 power;
        glm::vec3 ei;
        glm::vec3 ej;
        glm::vec3 normal;
        float area;	
        int nSamples;

    public:
        AreaLight(const glm::vec3& position, const glm::vec3& power, const glm::vec3& ei, const glm::vec3& ej, int nSamples);
        glm::vec3 radiance(const Scene* scene, const glm::vec3& point, glm::vec3* L) const override;
        glm::vec3 GetIrraciance() const override;
        glm::vec3 getSample(float* pdf, glm::vec3& ns) const;
        int getSampleCount() const { return nSamples; }
        float getArea() const { return area; }
        glm::vec3 getPower() const override { return power; }
};
#endif 