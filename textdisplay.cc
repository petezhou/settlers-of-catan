#include "textdisplay.h"
#include "tile.h"
#include <iostream>
#include "util.h"
using namespace std;

TextDisplay::~TextDisplay() {
    for (int i = 0; i < 42; i++){
	    delete [] theDisplay[i];
    }
    delete [] theDisplay;
}

//set up canvas, we need enough space to show all the chars
//returned by the other fucntions
TextDisplay::TextDisplay(vector<Tile*> tiles){
    theTiles = tiles;
    theDisplay = new char*[42];
    for (int i = 0; i < 42; i++){
        theDisplay[i] = new char[110];
        
    }
    
    for (int i = 0; i < 42; i++){
        for (int j = 0; j < 110; j++){
            theDisplay[i][j] = ' ';
        }

    }
}

char** TextDisplay::getDisplay(){
    return this->theDisplay;
}


//printing it to the console!!
ostream &operator<<(std::ostream &out, const TextDisplay &td){
    
    for (int i = 0; i < td.theTiles.size(); i++){
        Position pos = Util::getTilePosition(i);
        Tile *t = td.theTiles[i];
        t->draw(td.theDisplay, pos.getRow(), pos.getCol());
    }
    
    
    
    for (int i = 0; i < 42; i++){
        for (int j = 0; j < 110; j++){
            cout << td.theDisplay[i][j];
        }
        cout << endl;
    }
    
    
    return out;
}

