#pragma once

#include "glm/mat4x4.hpp"
#include <vector>
#include <memory>

class ITransformation
{
public:
    virtual ~ITransformation() = default;
    virtual glm::mat4 getModelMatrix() const = 0;
};

class CompositeTransformation : public ITransformation {
public:
    void add(std::shared_ptr<ITransformation> t);
    void remove(std::shared_ptr<ITransformation> t);
    glm::mat4 getModelMatrix() const override;
private:
    std::vector<std::shared_ptr<ITransformation>> children;
};
class CustomTransform : public ITransformation {
public:
    glm::mat4 getModelMatrix() const override {
        glm::mat4 m(1.0f);  
        m[3][3] = 20.0f;    
        return m;
    }
};