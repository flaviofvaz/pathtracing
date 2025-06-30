#include "pathtracer.h"
#include "camera.h"
#include "film.h"
#include "scene.h"
#include "light.h"
#include "instance.h"
#include "material.h"
#include "shape.h"
#include "transform.h"
#include "glm/glm.hpp"
#include <iostream>
#include <memory>

int main() 
{
    try {
        // create film
        const int width = 800;
        const int height = 600;
        const int numSamples = 64;
        const int dMax = 4;
        auto film = std::make_unique<Film>(glm::ivec2(width, height));

        // create camera
        glm::vec3 eye(0.0f, 2.5f, 7.0f);    // camera position
        glm::vec3 lookAt(0.0f, 1.0f, 0.0f); // look at point (center)
        glm::vec3 up(0.0f, 1.0f, 0.0f);     // up vector
        float fov = 50.0f;                  // field of view
        float focalDistance = 1.0f;         // distance from camera to look-at point (focal distance)
        auto camera = std::make_unique<Camera>(eye, lookAt, up, fov, focalDistance, width, height);

        // create scene
        auto scene = std::make_unique<Scene>();
        scene->setAmbientLight(glm::vec3(0.2, 0.2, 0.2));

        // create materials
        auto redMaterial = std::make_unique<PhongMaterial>(
            glm::vec3(0.8f, 0.1f, 0.1f)    // diffuse
        );

        auto blueMaterial = std::make_unique<PhongMaterial>(
            glm::vec3(0.1f, 0.1f, 0.8f)    // diffuse 
        );

        auto floorMaterial = std::make_unique<PhongMaterial>(
            glm::vec3(0.8f, 0.8f, 0.8f)    // diffuse (almost white)
        );

        // add lights to the scene
        glm::vec3 lightPosition(2.0f, 4.0f, 3.0f);
        
        // Add area light
        auto areaLight = std::make_unique<AreaLight>(
            glm::vec3(0.0f, 4.0f, 0.0f),                    // position
            glm::vec3(10.0f, 10.0f, 10.0f),   // power
            glm::vec3(1.0f, 0.0f, 0.0f),         // ei (x-axis)
            glm::vec3(0.0f, 0.0f, 1.0f),         // ej (z-axis)
            25                                 // number of samples
        );
        
        // Create a thin box to represent the area light
        auto areaLightBox = std::make_unique<Box>(
            glm::vec3(-0.5f, -0.05f, -0.5f),     // bMin (thin in y direction)
            glm::vec3(0.5f, 0.05f, 0.5f)         // bMax (thin in y direction)
        );

        auto areaLightInstance = std::make_unique<Instance>(std::move(areaLightBox));
        areaLightInstance->setLight(areaLight.get());
        areaLightInstance->translate(glm::vec3(0.0f, 4.0f, 0.0f));
        scene->addObject(std::move(areaLightInstance));
        
        // add objects to the scene
        auto sphere = std::make_unique<Sphere>(glm::vec3(0.5f, 1.0f, 0.0f), 1.0f); // larger red sphere
        auto sphereInstance = std::make_unique<Instance>(std::move(sphere));
        sphereInstance->setMaterial(redMaterial.get());
        sphereInstance->scale(glm::vec3(1.0f, 2.0f, 1.0f));
        scene->addObject(std::move(sphereInstance));

        // create floor
        auto floorBoxShape = std::make_unique<Box>(
            glm::vec3(-10.0f, -0.1f, -5.0f), // bMin
            glm::vec3(10.0f, 0.0f, 10.0f)     // bMax
        );
        auto floorInstance = std::make_unique<Instance>(std::move(floorBoxShape));
        floorInstance->setMaterial(floorMaterial.get());
        scene->addObject(std::move(floorInstance));

        // create blue sphere
        auto blueBox = std::make_unique<Box>(
            glm::vec3(-3.0f, 0.0f, -2.0f), // bMin
            glm::vec3(-1.0f, 2.0f, 1.0f));
        
        auto blueBoxInstance = std::make_unique<Instance>(std::move(blueBox));
        blueBoxInstance->setMaterial(blueMaterial.get());
        blueBoxInstance->translate(glm::vec3(0.0f, 1.0f, 0.0f));
        blueBoxInstance->rotate(45.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        scene->addObject(std::move(blueBoxInstance));

        // Create and run raytracer
        PathTracer pathtracer;
        pathtracer.render(film.get(), camera.get(), scene.get(), numSamples, dMax);

        // Save the rendered image
        if (!film->savePPM("output.ppm")) {
            std::cerr << "Failed to save image" << std::endl;
            return 1;
        }

        std::cout << "Rendering completed successfully!" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 