#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

class Subject;
class Tile;

class Observer {
    public:
	//criteria call this 
	virtual void notify(std::string typeOfResource) = 0;
	virtual ~Observer() {}
};

#endif
