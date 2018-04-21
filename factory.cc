#include <iostream>
#include "factory.h"
#include <string>
#include "loaded.h"
#include "dice.h"
#include "fair.h"

using namespace std;

Dice *Factory::createDice(string type) {
    if (type == "loaded") {
	Dice *d = new Loaded{};
	return d;
    } else {
	Dice *d = new Fair{};
	return d;
    }
}

