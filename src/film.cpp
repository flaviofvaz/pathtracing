#include "film.h"
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

Film::Film(glm::ivec2 resolution)
    : resolution(resolution)
    , image(resolution.x * resolution.y, glm::vec3(0.0f))
{
}

inline float Random()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec2 Film::pixelSampler(int i, int j)
{
    // Convert pixel coordinates to normalized device coordinates (0 to 1)
    float x = static_cast<float>(i + Random()) / resolution.x;
    float y = static_cast<float>(j + Random()) / resolution.y;
    return glm::vec2(x, y);
}

int Film::getHeight() const
{
    return resolution.y;
}

int Film::getWidth() const
{
    return resolution.x;
}

void Film::setValue(int i, int j, glm::vec3 pixelColor)
{
    int index = j * resolution.x + i;
    image[index] = pixelColor;
}

glm::vec3 Film::getValue(int i, int j) const
{
    int index = j * resolution.x + i;
    return image[index];
}

bool Film::savePPM(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    // Write PPM header
    file << "P3\n" << resolution.x << " " << resolution.y << "\n255\n";
    
    // Write pixel data
    for (int y = 0; y < resolution.y; y++) {
        for (int x = 0; x < resolution.x; x++) {
            glm::vec3 color = getValue(x, y);
            // Convert from [0,1] to [0,255] and clamp
            int r = static_cast<int>(glm::clamp(color.r * 255.0f, 0.0f, 255.0f));
            int g = static_cast<int>(glm::clamp(color.g * 255.0f, 0.0f, 255.0f));
            int b = static_cast<int>(glm::clamp(color.b * 255.0f, 0.0f, 255.0f));
            file << r << " " << g << " " << b << "\n";
        }
    }

    return true;
}