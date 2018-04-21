#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <string>


class Observer;

class Subject {
    std::vector<Observer *> observers;
public:
    virtual ~Subject();
    void attach(Observer *o);
    void notifyObservers();
    std::vector<Observer *> getObservers();
    virtual std::string getInfo() = 0;
};

#endif

