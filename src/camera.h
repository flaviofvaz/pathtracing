#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "ray.h"

class Camera
{
    private:
        float fov;
        float aspectRatio;
        float distance;
        glm::vec3 eye;
        glm::vec3 lookAt;
        glm::vec3 up;
        glm::mat4 viewMatrix;
        glm::mat4 inverseViewMatrix;

    public:
        Camera(const glm::vec3& eye, const glm::vec3& lookAt, const glm::vec3& up, 
               float fov, float distance, int width, int height);
        Ray generateRay(float x, float y) const;
};
#endif