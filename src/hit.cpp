#include "hit.h"

Hit::Hit()
    : type(InstanceType::NONE), backface(false), t(0.0f), position(0.0f), normal(0.0f) {}

Hit::Hit(float t, const glm::vec3& position, const glm::vec3& normal, bool backface)
    : type(InstanceType::NONE), backface(backface), t(t), position(position), normal(normal) {}

Hit::Hit(const Hit& other)
    : type(InstanceType::NONE), backface(other.backface), t(other.t), position(other.position), normal(other.normal)
{
    if (other.type == InstanceType::LIGHT) 
    {
        setLight(other.light);
    } 
    else if (other.type == InstanceType::MATERIAL) 
    {
        setMaterial(other.material);
    }
}

Hit& Hit::operator=(const Hit& other)
{
    if (this != &other) 
    {
        // Clean up current state
        if (type == InstanceType::LIGHT) 
        {
            light = nullptr;
        } 
        else if (type == InstanceType::MATERIAL) 
        {
            material = nullptr;
        }

        // Copy basic members
        backface = other.backface;
        t = other.t;
        position = other.position;
        normal = other.normal;

        // Copy union data
        if (other.type == InstanceType::LIGHT) 
        {
            setLight(other.light);
        } 
        else if (other.type == InstanceType::MATERIAL) 
        {
            setMaterial(other.material);
        }
        type = other.type;
    }
    return *this;
}

Hit::~Hit()
{
    light = nullptr;
    material = nullptr;
}

void Hit::setMaterial(const Material* material)
{
    if (type == InstanceType::LIGHT) 
    {
        light = nullptr;
    }
    this->material = material;
    type = material ? InstanceType::MATERIAL : InstanceType::NONE;
}

void Hit::setLight(const Light* light)
{
    if (type == InstanceType::MATERIAL) 
    {
        material = nullptr;
    }
    this->light = light;
    type = light ? InstanceType::LIGHT : InstanceType::NONE;
}

