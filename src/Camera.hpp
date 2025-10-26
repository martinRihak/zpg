#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "Subject.hpp"
#include "shaderProgram/ShaderProgram.hpp"

class ShaderProgram;

class Camera : public Subject 
{
private:
    glm::vec3 eye; // Position
    glm::vec3 target;
    glm::vec3 up;

    float alpha;
    float fi;
    float aspectRatio;

    void update();

public:
    Camera();
    ~Camera();

    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;
    glm::vec3 getUp() const;
    float getAlpha() const;
    float getFi() const;

    void setAspectRatio(float ar);
    glm::mat4 getCamera() const;
    glm::mat4 getProjectionMatrix() const;

    void setEye(const glm::vec3 &eye);
    void setAngels(float alpha, float fi);
    SubjectType getSubType() const override{return SubjectType::CAMERA;}
};
