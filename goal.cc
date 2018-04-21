#include "goal.h"
#include "subject.h"
#include "position.h"
#include "util.h"
#include "student.h"
#include "criteria.h"
#include <string>

using namespace std;

Goal::Goal(int pos):index(pos){
    owner=nullptr;
}

Goal::~Goal(){
}

bool Goal::ownedOrNot() {
    if (owner == nullptr) return false;
    return true;
}
 
Student *Goal::getOwner() {
    return owner;
}

void Goal::checkGoalNeighbours(bool *valid) {
    for(int i = 0; i < neighGoals.size(); ++i) {
        if (owner == (neighGoals[i])->getOwner()) {
            *valid = true;
        }
    }
}

//building the goal, if its setting up, then the student doesnt use resources to build.
void Goal::buildGoal(Student *s, bool isBegin) {
    if (isBegin == true) {
        owner = s;
	s->buildGoal(true, this->index);
    } else {
        bool validNeighbours = false;
        for(int i = 0; i < neighGoals.size(); ++i) { // only check those that are owned
            if ((neighGoals[i])->ownedOrNot()) {
                Student *otherOwner = (neighGoals[i])->getOwner();
                if (otherOwner->getStudentColour() == s->getStudentColour()) {
                    validNeighbours = true;
                }
            }
        }
        for(int i = 0; i < neighCriteria.size(); ++i) {
            if ((neighCriteria[i])->checkOwnedOrNot()) {
                Student *otherOwner = (neighCriteria[i])->getOwner();
                if (otherOwner->getStudentColour() == s->getStudentColour()) {
                    validNeighbours = true;
                }
            }
        }


        if ((owner == nullptr) && (validNeighbours == true)) {
            if (s->validGoal() == true) {
                owner = s;
                s->buildGoal(false, this->index);
            } else {
                throw string("You do not have enough resources.");
            }
        } else {
            throw string("You cannot build here.");
        }
    }
}

void Goal::attachGoal(Goal *g) {
    neighGoals.push_back(g);
}

void Goal::attachCriteria(Criteria *c) {
    neighCriteria.push_back(c);
}


//drawing the Textdisplay canvas
void Goal::draw(char ** canvas, int row, int col, int sequence){
    Position pos = Util::getEdgeRelativePosition(sequence);
    row += pos.getRow();
    col += pos.getCol();
    
    char tenthChar;
    char digitChar;
    if (owner == nullptr){
        tenthChar = Util::getTenthDigit(this->index);
        digitChar = Util::getDigit(this->index);
    } else {
        tenthChar = owner->getStudentColour()[0];
        digitChar = 'A';
    }
    
    
    if (sequence == 0 || sequence == 5) {
        canvas[row][col] = '-';
        canvas[row][col + 1] = '-';
        canvas[row][col + 2] = tenthChar;
        canvas[row][col + 3] = digitChar;
        canvas[row][col + 4] = '-';
        canvas[row][col + 5] = '-';
    }
    
    if (sequence == 1 || sequence == 4) {
        canvas[row][col] = '/';
        canvas[row + 1][col - 2] = tenthChar;
        canvas[row + 1][col - 1] = digitChar;
        canvas[row + 2][col - 3] = '/';
    }
    
    if (sequence == 2 || sequence == 3) {
        canvas[row][col] = '\\';
        canvas[row + 1][col + 1] = tenthChar;
        canvas[row + 1][col + 2] = digitChar;
        canvas[row + 2][col + 3] = '\\';
    }
}



