#include "instance.h"

Instance::Instance(std::unique_ptr<Shape> shape)
    : type(InstanceType::NONE), shape(std::move(shape)), transform(std::make_unique<Transform>()) {}

Instance::~Instance()
{
    light = nullptr;
    material = nullptr;
}

Instance::Instance(Instance&& other) noexcept
    : type(other.type), shape(std::move(other.shape)), transform(std::move(other.transform))
{
    if (type == InstanceType::LIGHT) 
    {
        light = other.light;
        other.light = nullptr;
    } 
    else if (type == InstanceType::MATERIAL) 
    {
        material = other.material;
        other.material = nullptr;
    }
    other.type = InstanceType::NONE;
}

Instance& Instance::operator=(Instance&& other) noexcept
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

        type = other.type;
        shape = std::move(other.shape);
        transform = std::move(other.transform);
        
        if (type == InstanceType::LIGHT) 
        {
            light = other.light;
            other.light = nullptr;
        } 
        else if (type == InstanceType::MATERIAL) 
        {
            material = other.material;
            other.material = nullptr;
        }
        other.type = InstanceType::NONE;
    }
    return *this;
}

void Instance::setMaterial(Material* material)
{
    if (type == InstanceType::LIGHT) 
    {
        light = nullptr;
    }
    this->material = material;
    type = material ? InstanceType::MATERIAL : InstanceType::NONE;
}

void Instance::setLight(Light* light)
{
    if (type == InstanceType::MATERIAL) 
    {
        material = nullptr;
    }
    this->light = light;
    type = light ? InstanceType::LIGHT : InstanceType::NONE;
}


std::unique_ptr<Hit> Instance::computeIntersection(const Ray& ray) const
{
    if (!shape) 
    {
        return nullptr;
    }

    auto hit = std::make_unique<Hit>();

    // Transform the ray to the local space of the instance
    glm::vec3 localRayOrigin = transform->inverseTransformPoint(ray.getRayOrigin());
    // Transform direction as a point, then subtract origin to get direction vector
    glm::vec3 localRayEnd = transform->inverseTransformPoint(ray.getRayOrigin() + ray.getRayDirection());
    glm::vec3 localRayDirection = glm::normalize(localRayEnd - localRayOrigin);
    
    Ray localRay(localRayOrigin, localRayDirection);
    if (shape->intersect(localRay, hit.get())) 
    {
        if (type == InstanceType::LIGHT) 
        {
            hit->setLight(light);
        } 
        else if (type == InstanceType::MATERIAL) 
        {
            hit->setMaterial(material);
        }

        // Transform the hit to the world space of the instance
        hit->position = transform->transformPoint(hit->position);
        hit->normal = transform->transformNormal(hit->normal);

        return hit;
    }
    return nullptr;
}

void Instance::translate(const glm::vec3& translation)
{
    transform->translate(translation);
}

void Instance::scale(const glm::vec3& scale)
{
    transform->scale(scale);
}

void Instance::rotate(float angle, const glm::vec3& axis)
{
    transform->rotate(angle, axis);
}
    