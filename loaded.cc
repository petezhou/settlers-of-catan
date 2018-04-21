#include <iostream>
#include "loaded.h"
#include "dice.h"
#include <sstream>

using namespace std;

Loaded::Loaded() {}

Loaded::~Loaded() {}

int Loaded::rollDie() {
    string s;
    cout << "> ";
    cin >> s;
    istringstream buffer(s);
    int numRolled;
    buffer >> numRolled;
    
    if ((numRolled >= 2) && (numRolled <= 12)) {
	return numRolled;
    } else {
	throw string("Invalid roll.");
    }
}

