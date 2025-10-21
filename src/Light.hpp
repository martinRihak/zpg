#pragma once

#include <glm/glm.hpp>
#include "Subject.hpp"
#include "Transformation/Transformation.hpp"

class Light : public Subject {
private:
    Transformation transformation;
    glm::vec3 diff;
    glm::vec3 spec;

public:
    Light(glm::vec3 pos, glm::vec3 d, glm::vec3 s);
    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3& pos);
    glm::vec3 getDiff() const;
    void setDiff(const glm::vec3& d);
    glm::vec3 getSpec() const;
    void setSpec(const glm::vec3& s);
    Transformation& getTransformation();
    SubjectType getSubType()const override {return SubjectType::LIGHT;}
    
    ~Light();
};