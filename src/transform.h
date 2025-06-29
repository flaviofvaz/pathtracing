#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Transform 
{
private:
    glm::mat4 transformMatrix;
    glm::mat4 inverseMatrix;
    glm::mat3 normalMatrix;

public:
    Transform() : transformMatrix(1.0f), inverseMatrix(1.0f), normalMatrix(1.0f) {}

    void setIdentity() 
    {
        transformMatrix = glm::mat4(1.0f);
        inverseMatrix = glm::mat4(1.0f);
        normalMatrix = glm::mat3(1.0f);
    }

    void translate(const glm::vec3& translation) 
    {
        transformMatrix = glm::translate(transformMatrix, translation);
        inverseMatrix = glm::inverse(transformMatrix);
        normalMatrix = glm::mat3(glm::transpose(inverseMatrix));
    }

    void scale(const glm::vec3& scale) 
    {
        transformMatrix = glm::scale(transformMatrix, scale);
        inverseMatrix = glm::inverse(transformMatrix);
        normalMatrix = glm::mat3(glm::transpose(inverseMatrix));
    }

    void rotate(float angle, const glm::vec3& axis) 
    {
        transformMatrix = glm::rotate(transformMatrix, glm::radians(angle), axis);
        inverseMatrix = glm::inverse(transformMatrix);
        normalMatrix = glm::mat3(glm::transpose(inverseMatrix));
    }

    glm::vec3 transformPoint(const glm::vec3& point) const 
    {
        glm::vec4 transformed = transformMatrix * glm::vec4(point, 1.0f);
        return glm::vec3(transformed) / transformed.w;
    }

    glm::vec3 transformVector(const glm::vec3& vector) const 
    {
        return glm::vec3(transformMatrix * glm::vec4(vector, 0.0f));
    }

    glm::vec3 transformNormal(const glm::vec3& normal) const 
    {
        return glm::normalize(normalMatrix * normal);
    }

    glm::vec3 inverseTransformPoint(const glm::vec3& point) const 
    {
        glm::vec4 transformed = inverseMatrix * glm::vec4(point, 1.0f);
        return glm::vec3(transformed) / transformed.w;
    }

    glm::vec3 inverseTransformVector(const glm::vec3& vector) const 
    {
        return glm::vec3(inverseMatrix * glm::vec4(vector, 0.0f));
    }

    const glm::mat4& getMatrix() const { return transformMatrix; }
    const glm::mat4& getInverseMatrix() const { return inverseMatrix; }
    const glm::mat3& getNormalMatrix() const { return normalMatrix; }
};

#endif // TRANSFORM_H 