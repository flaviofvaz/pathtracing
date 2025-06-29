#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "camera.h"
#include "film.h"

class RayTracer
{
    public:
        RayTracer() {};
        void render(Film* Film, Camera* camera, Scene* scene, float numSamples, int dMax);
};
#endif
