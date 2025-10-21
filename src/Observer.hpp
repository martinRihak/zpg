#pragma once
#include "Subject.hpp"
class Subject;
class Observer
{
public:
    virtual ~Observer() = default;
    virtual void notify(Subject* subject) = 0;
};

