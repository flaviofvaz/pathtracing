#include "shape.h"
#include "ray.h"
#include "hit.h"

#include <glm/glm.hpp>

#define EPSILON 1e-3f

Sphere::Sphere(const glm::vec3& center, float radius)
    : center(center), radius(radius) {}

bool Sphere::intersect(const Ray& ray, Hit* hit) const
{
    const glm::vec3& rayDirection = ray.getRayDirection();
    const glm::vec3& rayOrigin = ray.getRayOrigin();
    glm::vec3 rayCenterDistance = rayOrigin - center;

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayDirection, rayCenterDistance);
    float c = glm::dot(rayCenterDistance, rayCenterDistance) - radius * radius;

    float delta = b*b - 4*a*c;
    if(delta < 0) // no intersection
    {
        return false;
    }

    float t1 = (-b - std::sqrt(delta))/(2*a);
    float t2 = (-b + std::sqrt(delta))/(2*a);

    float t;
    if (t1 >= EPSILON) // avoiding auto intersection
    {
        t = t1;
    }
    else if (t2 >= EPSILON)
    {
        t = t2;
    }
    else
    {
        return false;
    }
    
    hit->t = t;
    hit->position = rayOrigin + t * rayDirection;      

    glm::vec3 normal = (hit->position - center) / radius; // normalizing
    if (t1 < 0 || t2 < 0)
    {
        hit->normal = -normal;
        hit->backface = true;
    }
    else
    {
        hit->normal = normal;
        hit->backface = false;
    }
    return true;
}

Box::Box(const glm::vec3& bMin, const glm::vec3& bMax)
    : bMin(bMin), bMax(bMax) {}

bool Box::intersect(const Ray& ray, Hit* hit) const
{
    const glm::vec3& rayOrigin = ray.getRayOrigin();
    const glm::vec3& rayDirection = ray.getRayDirection();

    glm::vec3 invRayDirection;
    float t_near_slabs[3];
    float t_far_slabs[3];

    // X slab
    if (std::abs(rayDirection.x) < EPSILON) 
    { // Ray parallel to X-axis
        if (rayOrigin.x < bMin.x || rayOrigin.x > bMax.x) return false; // Misses if origin outside slab
        t_near_slabs[0] = -std::numeric_limits<float>::infinity();
        t_far_slabs[0] = std::numeric_limits<float>::infinity();
    } 
    else 
    {
        invRayDirection.x = 1.0f / rayDirection.x;
        float t0_x = (bMin.x - rayOrigin.x) * invRayDirection.x;
        float t1_x = (bMax.x - rayOrigin.x) * invRayDirection.x;
        t_near_slabs[0] = glm::min(t0_x, t1_x);
        t_far_slabs[0] = glm::max(t0_x, t1_x);
    }

    // Y slab
    if (std::abs(rayDirection.y) < EPSILON) 
    { // Ray parallel to Y-axis
        if (rayOrigin.y < bMin.y || rayOrigin.y > bMax.y) return false; // Misses if origin outside slab
        t_near_slabs[1] = -std::numeric_limits<float>::infinity();
        t_far_slabs[1] = std::numeric_limits<float>::infinity();
    } 
    else 
    {
        invRayDirection.y = 1.0f / rayDirection.y;
        float t0_y = (bMin.y - rayOrigin.y) * invRayDirection.y;
        float t1_y = (bMax.y - rayOrigin.y) * invRayDirection.y;
        t_near_slabs[1] = glm::min(t0_y, t1_y);
        t_far_slabs[1] = glm::max(t0_y, t1_y);
    }

    // Z slab
    if (std::abs(rayDirection.z) < EPSILON) 
    { // Ray parallel to Z-axis
        if (rayOrigin.z < bMin.z || rayOrigin.z > bMax.z) return false; // Misses if origin outside slab
        t_near_slabs[2] = -std::numeric_limits<float>::infinity();
        t_far_slabs[2] = std::numeric_limits<float>::infinity();
    } 
    else 
    {
        invRayDirection.z = 1.0f / rayDirection.z;
        float t0_z = (bMin.z - rayOrigin.z) * invRayDirection.z;
        float t1_z = (bMax.z - rayOrigin.z) * invRayDirection.z;
        t_near_slabs[2] = glm::min(t0_z, t1_z);
        t_far_slabs[2] = glm::max(t0_z, t1_z);
    }

    float t_enter = glm::max(glm::max(t_near_slabs[0], t_near_slabs[1]), t_near_slabs[2]);
    float t_exit = glm::min(glm::min(t_far_slabs[0], t_far_slabs[1]), t_far_slabs[2]);

    if (t_enter > t_exit || t_exit < EPSILON) 
    {
        return false; // Misses the box or intersection is behind/too close
    }

    float t_final = t_enter;
    bool ray_starts_inside = false;

    if (t_enter < EPSILON) 
    { // Ray origin is inside the box or on its entry surface
        if (t_exit < EPSILON)
        { 
            return false; // Entire intersection (entry and exit) is behind/too close
        }
        t_final = t_exit;    // Use the exit point as the intersection
        ray_starts_inside = true;
    }

    if (t_final > EPSILON) 
    {
        hit->t = t_final;
        hit->position = rayOrigin + t_final * rayDirection;

        glm::vec3 p = hit->position;
        glm::vec3 outNormal(0.0f);
        const float norm_epsilon = 1e-4f; // Tolerance for comparing point to face

        if(std::abs(p.x - bMin.x) < norm_epsilon)
        { 
            outNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        else if(std::abs(p.x - bMax.x) < norm_epsilon)
        {
            outNormal = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else if(std::abs(p.y - bMin.y) < norm_epsilon)
        {
            outNormal = glm::vec3(0.0f, -1.0f, 0.0f);
        }
        else if(std::abs(p.y - bMax.y) < norm_epsilon){
            outNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        else if(std::abs(p.z - bMin.z) < norm_epsilon)
        {
            outNormal = glm::vec3(0.0f, 0.0f, -1.0f);
        }
        else if(std::abs(p.z - bMax.z) < norm_epsilon)
        {
            outNormal = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        // Apply the consistent normal and backface setup discussed previously:
        glm::vec3 normal = glm::normalize(outNormal); // Ensure normalized if not already unit

        if (ray_starts_inside) 
        {
            hit->backface = true; 
            hit->normal = -normal;
        } 
        else 
        {
            hit->backface = false; 
            hit->normal = normal;
        }
        
        return true;
    }

    return false;
}