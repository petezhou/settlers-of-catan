#include "position.h"


Position::Position()
{
}

Position::Position(int row, int col)
{
    this->row = row;
    this-> col = col;
}


Position::~Position(){}

void Position::setRow(int row)
{
    this->row = row;
}

void Position::setCol(int col)
{
    this->col = col;
}

int Position::getRow()
{
    return this->row;
}

int Position::getCol()
{
    return this->col;
}

