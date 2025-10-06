#include "ITransformation.hpp"
#include <algorithm>

void CompositeTransformation::add(std::shared_ptr<ITransformation> t) {
    if (!t) return;
    children.push_back(t);
}

void CompositeTransformation::remove(std::shared_ptr<ITransformation> t) {
    if (!t) return;
    auto it = std::find(children.begin(), children.end(), t);
    if (it != children.end()) children.erase(it);
}

glm::mat4 CompositeTransformation::getModelMatrix() const {
    glm::mat4 result(1.0f);
    for (const auto& child : children) {
        if (child) result = result * child->getModelMatrix();
    }
    return result;
}