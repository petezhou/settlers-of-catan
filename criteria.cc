#include "criteria.h"
#include "subject.h"
#include "position.h"
#include "util.h"
#include "student.h"
#include "goal.h"
#include <string>

using namespace std;

Criteria::Criteria(int index):index(index){
    owner = nullptr;
}


Criteria::~Criteria(){
}

//finds all the neighbouring critera, you cannot build there.
void Criteria::attachCritera(Criteria *c) {
    neighCriteria.push_back(c);
}    

//find neighbouring goal, we need on to build
void Criteria::attachGoal(Goal *g) {
    neighGoal.push_back(g);
}


//gets notification from tile, the tile produces resources
void Criteria::notify(std::string typeOfResource){
    int howMany;
    if (criteriaType == "ASSIGNMENT") {
        howMany = 1;
    } else if (criteriaType == "MIDTERM") {
        howMany = 2;
    } else if (criteriaType == "EXAM"){
        howMany = 3;
    } else {
        howMany = 0;
    } 
    if (howMany != 0) owner->notifyStudent(typeOfResource, howMany);
}

// getters
string Criteria::getType(){
    return this->criteriaType;
}

Student *Criteria::getOwner(){ 
    return this->owner;
}

//displaying criteria onto our canvas, 
void Criteria::draw(char** canvas, int row, int col, int sequence){
    //get the position on our canvas
    Position pos = Util::getVertexRelativePosition(sequence);
    row += pos.getRow();
    col += pos.getCol();
    
    
    char tenthChar = ' ';
    char digitChar = ' ';
    //if owner is not defined, then print the index, else Owner and type
    if (owner == nullptr){
        tenthChar = Util::getTenthDigit(this->index);
        digitChar = Util::getDigit(this->index);
    } else {
        if (criteriaType == "ASSIGNMENT"){
            tenthChar = owner->getStudentColour()[0];
            digitChar = 'A';
        } else if (criteriaType == "MIDTERM"){
            tenthChar = owner->getStudentColour()[0];
            digitChar = 'M';
        } else if (criteriaType =="EXAM") {
            tenthChar = owner->getStudentColour()[0];
            digitChar = 'E';
        } else {
            tenthChar = 'Q';
            digitChar = 'Q';
        }
    }
    //decorative
    canvas[row][col] =  '|';
    canvas[row][col + 1] = tenthChar;
    canvas[row][col + 2] = digitChar;
    canvas[row][col + 3] = '|';
}


//is it owned?
bool Criteria::checkOwnedOrNot() {
    if (owner == nullptr) return false;
    return true;
}

//do we have neighbour criteria?
bool Criteria::checkCriteriaNeighbours() {
    for(int i = 0; i < neighCriteria.size(); ++i) {
	if (neighCriteria[i]->checkOwnedOrNot() == true) {
	    return false;
	}
    }
    return true;
}


//this is actually placing a criteria down
void Criteria::buildCriteria(Student *s, bool isBegin) { // add checkCriteriaNeighbours
    //at the beginning we dont need to use resources to build
    if ((isBegin == true) && (owner == nullptr) && (this->checkCriteriaNeighbours() == true)) {
        owner = s;
        s->buildAssignment(true, this->index);
        criteriaType = "ASSIGNMENT";
	ownedOrNot = true;
    } else if (isBegin == true) {
	throw string("You cannot build here.");
    } else if (isBegin == false) {
        bool validNeighbours = false;
        for(int i = 0; i < neighGoal.size(); ++i) { // only check those that are owned
            if ((neighGoal[i])->ownedOrNot()) {
		Student *otherOwner = (neighGoal[i])->getOwner();
		if (otherOwner->getStudentColour() == s->getStudentColour()) {
                    validNeighbours = true;
		}
            }
        }
        if ((owner == nullptr) && validNeighbours && (this->checkCriteriaNeighbours() == true)) {
            if (s->validAssignment() == true) {
                owner = s;
         	//students need to spend resources.        
                s->buildAssignment(false, this->index);
                criteriaType = "ASSIGNMENT";
		ownedOrNot = true;
            } else {
                throw string("You do not have enough resources.");
            }
        } else {
            throw string("You cannot build here.");
        }
    } else {
        throw string("You cannot build here.");
    }
}


//depending on what criteria is right now, we change the criteria to the right type
void Criteria::improveCriteria(Student *s, bool isBegin) {
    if ((isBegin == true) && (owner == s)) {
	if (criteriaType == "ASSIGNMENT") {
	    //student needs to pay resources.
	    s->buildMidterm(true, this->index);
	    criteriaType = "MIDTERM";
	} else if (criteriaType == "MIDTERM") {
	    s->buildExam(true, this->index);
	    criteriaType = "EXAM";
	}
    } else if (criteriaType == "ASSIGNMENT") {
        if ((s->validMidterm() == true) && (owner == s)) {
            s->buildMidterm(false, this->index);
            criteriaType = "MIDTERM";
        } else {
            throw string("You do not have enough resources.");
        }
    } else if (criteriaType == "MIDTERM") {
        if ((s->validExam() == true) && (owner == s)) {
            s->buildExam(false, this->index);
            criteriaType = "EXAM";
        } else {
            throw string("You do not have enough resources.");
        }
    } else {
        throw string("You cannot build here.");
    }
}

