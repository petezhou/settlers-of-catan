#include "tile.h"
#include "criteria.h"
#include "goal.h"
#include "util.h"
#include "student.h"

using namespace std ;


Tile::Tile(int index, string resource, int val) {
    this->index = index;
    this->val = val;
    this->resource = resource;
    isGeeseOver = false;
}

string Tile::getInfo() {
    return resource;
}

Tile::~Tile(){
}


int Tile::getVal() {
    return val ;
}

int Tile::getPos() {
    return index;
}

string Tile::getName() {
    return resource;
}

Goal *Tile::getGoal(int idx)
{
    return this->goals[idx];
}

void Tile::setGoal(Goal * goal, int idx)
{
    this->goals[idx] = goal;
}


void Tile::setGeese(bool g){
    isGeeseOver = g;
}

bool Tile::getGeeseOver() {
    return isGeeseOver;
}


//every owned criterion surrounding the tile
std::vector<Student *> Tile::ownedCriterion(Student *theif){
    vector<Student *> owners;
    
    for (int i = 0; i < 6; i++){
        Criteria *c = dynamic_cast<Criteria*>(this->getObservers()[i]);
        if (c->getOwner() != nullptr && c->getOwner() != theif){
        	owners.push_back(c->getOwner());
        }
    }
    return owners;
}


//printing to canvas
void Tile::draw(char** canvas, int row, int col){
    
    for (int sequence = 0; sequence < 6; sequence++) {
        Criteria *c = dynamic_cast<Criteria*>(this->getObservers()[sequence]);
        c->draw(canvas, row, col, sequence);
        this->goals[sequence]->draw(canvas, row, col, sequence);
    }
    //show index
    canvas[row + 2][col + 6] = Util::getTenthDigit(index);
    canvas[row + 2][col + 7] = Util::getDigit(index);
    //show resource
    for (int i = 0; i < resource.size(); i++){
        canvas[row + 3][col + 4 + i] = resource[i];
    }
    //show val of die roll
    if (val != 7){
        canvas[row + 4][col + 6] = Util::getTenthDigit(val);
        canvas[row + 4][col + 7] = Util::getDigit(val);
    }
    //show if geese is here
    string geese = "GEESE";
    if (isGeeseOver){
        for (int i = 0; i < 5; i++){
            canvas[row + 5][col + 4 + i] = geese[i];
        }
    } else {
        for (int i = 0; i < 5; i++){
            canvas[row + 5][col + 4 + i] = ' ';
        }
    }
    
}



