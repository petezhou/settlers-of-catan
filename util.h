#ifndef UTIL_H
#define UTIL_H

#include "position.h"

class Util{
    
public:
    static void release();
    static char getTenthDigit(int value);
    static char getDigit(int value);
    static Position getTilePosition(int tileIdx);
    static Position getVertexRelativePosition(int vertexIdx);
    static Position getEdgeRelativePosition(int edgeIdx);
    static int* getTileVertexDistribution(int tileIdx);
    static int* getTileEdgeDistribution(int tileIdx);
};

#endif

