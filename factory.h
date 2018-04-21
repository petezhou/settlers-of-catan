#ifndef FACTORY_H
#define FACTORY_H

#include <iostream>

class Dice;

class Factory {
    public:
//	Dice *destroyDice();
	Dice *createDice(std::string type);
};

#endif
