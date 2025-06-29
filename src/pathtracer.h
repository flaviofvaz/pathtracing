#ifndef PATHTRACER_H
#define PATHTRACER_H

#include "scene.h"
#include "camera.h"
#include "film.h"

class PathTracer
{
    public:
        PathTracer() {};
        void render(Film* Film, Camera* camera, Scene* scene, float numSamples, int dMax);
};
#endif
