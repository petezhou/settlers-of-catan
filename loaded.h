#ifndef LOADED_H
#define LOADED_H

#include "dice.h"

class Dice;

class Loaded : public Dice {
    public:
        Loaded();
	~Loaded();
        int rollDie() override;
};

#endif
