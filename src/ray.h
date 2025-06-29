#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray
{   
    private:
        glm::vec3 direction;
        glm::vec3 origin;
    public:
        Ray(const glm::vec3& origin, const glm::vec3& direction);
        const glm::vec3& getRayDirection() const { return direction; }
        const glm::vec3& getRayOrigin() const { return origin; }
};
#endif