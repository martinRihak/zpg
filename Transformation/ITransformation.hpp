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
