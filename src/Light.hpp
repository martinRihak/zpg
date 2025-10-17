#pragma once

#include <glm/glm.hpp>
#include "Subject.hpp"
#include "Camera.hpp"
class Camera;
class Light : public Subject
{
private:
    glm::vec3 position;
    glm::vec3 diff;
    glm::vec3 spec;

public:
    Light(glm::vec3 pos, glm::vec3 diff, glm::vec3 spec);

    glm::vec3 getPosition() const;
    glm::vec3 getDiff() const;
    glm::vec3 getSpec() const;
    ~Light();
};