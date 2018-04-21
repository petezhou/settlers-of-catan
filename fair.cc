#include <iostream>
#include "fair.h"
#include "dice.h"
#include <stdlib.h>
#include <ctime>

using namespace std;

Fair::Fair() {}

Fair::~Fair() {}

int Fair::rollDie() {
    srand(time(NULL));
    return ((rand() % 6) + 1) + ((rand() % 6) + 1);
}

