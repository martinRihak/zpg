#pragma once

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void notify() = 0;
};

