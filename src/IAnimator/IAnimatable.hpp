// IAnimatable.hpp
#pragma once
#include "../Transformation/Transformation.hpp"

class IAnimatable {
public:
    virtual ~IAnimatable() = default;
    virtual Transformation& getTransformation() = 0;
    virtual const Transformation& getTransformation() const = 0;
};
