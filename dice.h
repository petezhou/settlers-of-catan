#ifndef DICE_H
#define DICE_H

class Dice {
    public:
	virtual ~Dice() {}
	virtual int rollDie() = 0;
};

#endif

