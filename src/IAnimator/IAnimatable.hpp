// IAnimatable.hpp
#pragma once
#include "../Transformation/Transformation.hpp"

class IAnimatable {
public:
    virtual ~IAnimatable() = default;
    virtual void destroy() = 0;
    virtual Transformation& getTransformation() = 0;
    virtual const Transformation& getTransformation() const = 0;
};
