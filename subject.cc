#include "subject.h"
#include "observer.h"
#include <iostream>

using namespace std;

Subject::~Subject() {}

void Subject::attach(Observer *o){
    observers.emplace_back(o);
}

void Subject::notifyObservers(){
    for (auto &ob : observers){
        ob->notify(this->getInfo());
    }
}

vector<Observer *> Subject::getObservers(){
    return this->observers;
}

