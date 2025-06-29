#include "raytracer.h"
#include "glm/glm.hpp"

void RayTracer::render(Film* film, Camera* camera, Scene* scene, float numSamples, int dMax)
{
    for(int j = 0; j < film->getHeight(); j++)
    {
        for(int i = 0; i < film->getWidth(); i++)
        {
            glm::vec3 color(0.0f);
            for(int s = 0; s < numSamples; s++)
            {
                // Sample pixel position
                glm::vec2 sampledPixel = film->pixelSampler(i, j);
                
                // Generate ray for this pixel
                Ray ray = camera->generateRay(sampledPixel.x, sampledPixel.y);
            
                // trace ray and get color
                color += scene->tracePath(ray, dMax);
            }                
            // Set pixel color
            film->setValue(i, j, color / numSamples);
        }
    }
}