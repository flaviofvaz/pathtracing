#include "camera.h"
#include "ray.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& eye, const glm::vec3& lookAt, const glm::vec3& up, 
               float fov, float distance, int width, int height)
    : eye(eye)
    , lookAt(lookAt)
    , up(up)
    , fov(fov)
    , distance(distance)
    , aspectRatio(static_cast<float>(width) / height)
{
    viewMatrix = glm::lookAt(eye, lookAt, up);
    inverseViewMatrix = glm::inverse(viewMatrix);
}

Ray Camera::generateRay(float Xn, float Yn) const
{
    // calculate view plane dimensions at the focal distance
    float deltaV = distance * std::tan(glm::radians(fov) / 2.0f);
    float deltaU = deltaV * aspectRatio;
    
    // calculate ray direction in camera space
    glm::vec4 p;
    p.x = -deltaU + 2.0f * deltaU * Xn;
    //p.y = -deltaV + 2.0f * deltaV * Yn;
    p.y = deltaV - 2.0f * deltaV * Yn;
    p.z = -distance;
    p.w = 1.0f;
    
    glm::vec4 o = inverseViewMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 t = inverseViewMatrix * p;

    glm::vec3 rayOrigin = glm::vec3(o) / o.w;
    glm::vec3 rayDirection = glm::normalize(glm::vec3(t) / t.w - rayOrigin);

    return Ray(rayOrigin, rayDirection);
}