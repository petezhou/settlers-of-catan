#ifndef textdisplay_hpp
#define textdisplay_hpp

#include <vector>
#include "observer.h"

class Tile;

class TextDisplay {
    std::vector<Tile*> theTiles;
    char** theDisplay;
public:
    TextDisplay(std::vector<Tile*>);
    ~TextDisplay();
    char** getDisplay();
    
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif

