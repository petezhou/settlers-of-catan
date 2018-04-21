#include "util.h"

static int* vertexDistribution[] = {
        new int[6] {0, 1, 3, 4, 8, 9},
        new int[6] {2, 3, 7, 8, 13, 14},
        new int[6] {4, 5, 9, 10, 15, 16},
        new int[6]{ 6, 7, 12, 13, 18, 19 },
        new int[6] {8, 9, 14, 15, 20, 21},
        new int[6] {10, 11, 16, 17, 22, 23},
        new int[6] {13, 14, 19, 20, 25, 26},
        new int[6] {15, 16, 21, 22, 27, 28},
        new int[6] {18, 19, 24, 25, 30, 31},
        new int[6] {20, 21, 26, 27, 32, 33},
        new int[6] {22, 23, 28, 29, 34, 35},
        new int[6] {25, 26, 31, 32, 37, 38},
        new int[6] {27, 28, 33, 34, 39, 40},
        new int[6] {30, 31, 36, 37, 42, 43},
        new int[6] {32, 33, 38, 39, 44, 45},
        new int[6] {34, 45, 40, 41, 46, 47},
        new int[6] {37, 38, 43, 44, 48, 49},
        new int[6] {39, 40, 45, 46, 50, 51},
        new int[6] {44, 45, 49, 50, 52, 53},
    };



  static int* edgeDistribution[] = {
        new int[6] {0, 1, 2, 6, 7, 10},
        new int[6] {3, 5, 6, 13, 14, 18},
        new int[6] {4, 7, 8, 15, 16, 19},
        new int[6]{ 9, 12, 13, 20, 21, 26 },
        new int[6] {10, 14, 15, 22, 23},
        new int[6] {11, 16, 17, 24, 25, 28},
        new int[6] {18, 21, 22, 30, 31, 35},
        new int[6] {19, 23, 24, 32, 33, 36},
        new int[6] {26, 29, 30, 37, 38, 43},
        new int[6] {27, 31, 32, 29, 40, 44},
        new int[6] {28, 33, 34, 41, 42, 45},
        new int[6] {35, 38, 39, 47, 48, 52},
        new int[6] {36, 40, 41, 49, 50, 53},
        new int[6] {43, 46, 47, 54, 55, 60},
        new int[6] {44, 48, 49, 56, 57, 61},
        new int[6] {45, 50, 51, 58, 59, 62},
        new int[6] {52, 55, 56, 63, 64, 67},
        new int[6] {53, 57, 58, 65, 66, 68},
        new int[6] {61, 64, 65, 69, 70, 71},
    };





char Util::getTenthDigit(int value)
{
    int tenth = value / 10;
    return tenth > 0 ? tenth + '0' : ' ';
}

char Util::getDigit(int value)
{
    int digit = value % 10;
    return digit + '0';
}


//vetertices for 19 tiles to be layout on canvas
Position Util::getTilePosition(int tileIdx)
{
    static Position tilePositions[] = {
        Position(0, 40),
        Position(4, 24),
        Position(4, 56),
        Position(8, 8),
        Position(8, 40),
        Position(8, 72),
        Position(12, 24),
        Position(12, 56),
        Position(16, 8),
        Position(16, 40),
        Position(16, 72),
        Position(20, 24),
        Position(20, 56),
        Position(24, 8),
        Position(24, 40),
        Position(24, 72),
        Position(28, 24),
        Position(28, 56),
        Position(32, 40),
    };
    
    return tilePositions[tileIdx];
}



//6 relative vertex position within a tile
Position Util::getVertexRelativePosition(int vertexIdx)
{
    static Position vertexRelativePositions[] = {
        Position(0, 0),
        Position(0, 10),
        Position(4, -6),
        Position(4, 16),
        Position(8, 0),
        Position(8, 10),
    };
    
    return vertexRelativePositions[vertexIdx];
}


//6 realteive edge position within a tile
Position Util::getEdgeRelativePosition(int edgeIdx)
{
    static Position edgeRelativePositions[] = {
        Position(0, 4),
        Position(1, 0),
        Position(1, 13),
        Position(5, -3),
        Position(5, 16),
        Position(8, 4),
    };
    
    return edgeRelativePositions[edgeIdx];
}




int * Util::getTileVertexDistribution(int tileIdx)
{
/*
    static int* vertexDistribution[] = {
        new int[6] {0, 1, 3, 4, 8, 9},
        new int[6] {2, 3, 7, 8, 13, 14},
        new int[6] {4, 5, 9, 10, 15, 16},
        new int[6]{ 6, 7, 12, 13, 18, 19 },
        new int[6] {8, 9, 14, 15, 20, 21},
        new int[6] {10, 11, 16, 17, 22, 23},
        new int[6] {13, 14, 19, 20, 25, 26},
        new int[6] {15, 16, 21, 22, 27, 28},
        new int[6] {18, 19, 24, 25, 30, 31},
        new int[6] {20, 21, 26, 27, 32, 33},
        new int[6] {22, 23, 28, 29, 34, 35},
        new int[6] {25, 26, 31, 32, 37, 38},
        new int[6] {27, 28, 33, 34, 39, 40},
        new int[6] {30, 31, 36, 37, 42, 43},
        new int[6] {32, 33, 38, 39, 44, 45},
        new int[6] {34, 45, 40, 41, 46, 47},
        new int[6] {37, 38, 43, 44, 48, 49},
        new int[6] {39, 40, 45, 46, 50, 51},
        new int[6] {44, 45, 49, 50, 52, 53},
    };
  */  
    return vertexDistribution[tileIdx];
}

int * Util::getTileEdgeDistribution(int tileIdx)
{
/*
    static int* edgeDistribution[] = {
        new int[6] {0, 1, 2, 6, 7, 10},
        new int[6] {3, 5, 6, 13, 14, 18},
        new int[6] {4, 7, 8, 15, 16, 19},
        new int[6]{ 9, 12, 13, 20, 21, 26 },
        new int[6] {10, 14, 15, 22, 23},
        new int[6] {11, 16, 17, 24, 25, 28},
        new int[6] {18, 21, 22, 30, 31, 35},
        new int[6] {19, 23, 24, 32, 33, 36},
        new int[6] {26, 29, 30, 37, 38, 43},
        new int[6] {27, 31, 32, 29, 40, 44},
        new int[6] {28, 33, 34, 41, 42, 45},
        new int[6] {35, 38, 39, 47, 48, 52},
        new int[6] {36, 40, 41, 49, 50, 53},
        new int[6] {43, 46, 47, 54, 55, 60},
        new int[6] {44, 48, 49, 56, 57, 61},
        new int[6] {45, 50, 51, 58, 59, 62},
        new int[6] {52, 55, 56, 63, 64, 67},
        new int[6] {53, 57, 58, 65, 66, 68},
        new int[6] {61, 64, 65, 69, 70, 71},
    };
  */  
    return edgeDistribution[tileIdx];
}

void Util::release(){
    for (int i =0; i < 19; i++){
	delete [] vertexDistribution[i];
    }

    for (int i = 0; i <19; i++){
	delete [] edgeDistribution[i];
    }
}
  



	
