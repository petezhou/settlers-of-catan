#ifndef FAIR_H
#define FAIR_H

#include "dice.h"

class Dice;

class Fair : public Dice {
public:
    Fair();
    ~Fair();
    int rollDie() override;
};

#endif
