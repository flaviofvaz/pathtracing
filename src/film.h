#ifndef FILM_H
#define FILM_H

#include <glm/glm.hpp>
#include <vector> 
#include <string>

class Film
{
    private:
        glm::ivec2 resolution;
        std::vector<glm::vec3> image;

    public:
        Film(glm::ivec2 resolution);
        glm::vec2 pixelSampler(int i, int j);
        int getHeight() const;
        int getWidth() const;
        void setValue(int i, int j, glm::vec3 pixelColor);
        glm::vec3 getValue(int i, int j) const;
        bool savePPM(const std::string& filename) const;
};

#endif