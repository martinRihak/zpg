#pragma once

#include "Observer.hpp"
#include <vector>
#include <algorithm>
class Subject
{
private:
    std::vector<Observer *> observers;

public:
    void attach(Observer *o)
    {
        observers.push_back(o);
    }
    void detach(Observer *o)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
    }
    void notifyAll()
    {
        for (Observer *o : observers)
        {
            o->notify();
        }
    }
};
