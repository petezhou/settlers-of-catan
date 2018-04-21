#include "student.h"
#include "criteria.h"
#include <iostream>
#include "dice.h"
#include <map>
#include <sstream>
#include <string>

using namespace std;
//setting things up with 0
Student::Student(std::string colour):studentColour(colour), victoryPoints{0},
    numCaf{0}, numLab{0}, numLec{0}, numStu{0}, numTut{0} {
    criterionOwned.clear();
    criteriaType.clear();
    goalsOwned.clear();
}


Student::~Student(){
    delete fairDice;
    delete loadedDice;
}

//the next 2 functions print student data to save file
void printSaveConverter(string *studentData, int num, bool spaceOrNot) {
    stringstream ss;
    ss << num;
    string str = ss.str();
    (*studentData) += str;
    ss.clear();
    if (spaceOrNot == true) {
	(*studentData) += " ";
    }
}

string Student::printSave() {
    string studentData = "\0";
    string *pstudentData = &studentData;
    printSaveConverter(pstudentData, numCaf, true);
    printSaveConverter(pstudentData, numLab, true);
    printSaveConverter(pstudentData, numLec, true);
    printSaveConverter(pstudentData, numStu, true);
    printSaveConverter(pstudentData, numTut, true);
    studentData += "g ";
    for(int i = 0; i < goalsOwned.size(); ++i) { 
        printSaveConverter(pstudentData, goalsOwned[i], true);
    }
    studentData += "c ";
    for(int i = 0; i < criterionOwned.size(); ++i) {
	if (i == criterionOwned.size() - 1) {
	    printSaveConverter(pstudentData, criterionOwned[i], true);
	    printSaveConverter(pstudentData, criteriaType[i], false);
	} else {
	    printSaveConverter(pstudentData, criterionOwned[i], true);
	    printSaveConverter(pstudentData, criteriaType[i], true);
        }
    }
    return studentData;
}


//reset 0
void Student::resetStudent() {
    numCaf = 0;
    numTut = 0;
    numStu = 0;
    numLec = 0;
    numLab = 0;
    victoryPoints = 0;
    typeOfDice = "fair";
    criterionOwned.clear();
    criteriaType.clear();            
    goalsOwned.clear();
}

//set resources to whatever we want
void Student::setResources(vector<int> resources){
    numCaf = resources[0];
    numLab = resources[1];
    numLec = resources[2];
    numStu = resources[3];
    numTut = resources[4];
}


bool Student::isWon(){
    return this->victoryPoints == 10;
}

//when student get this the resource will increase/decrease by 1 accordingly
void Student::receiveTrade(string give, string take) {
    if (give == "CAFFEINE") {
        numCaf = numCaf - 1;
    } else if (give == "STUDY") {
        numStu = numStu - 1;
    } else if (give == "LAB") {
        numLab = numLab - 1;
    } else if (give == "LECTURE") {
        numLec = numLec - 1;
    } else if (give == "TUTORIAL") {
        numTut = numTut - 1;
    }
    
    if (take == "CAFFEINE") {
        numCaf = numCaf + 1;
    } else if (take == "STUDY") {
        numStu = numStu + 1;
    } else if (take == "LAB") {
        numLab = numLab + 1;
    } else if (take == "LECTURE") {
        numLab = numLab + 1;
    } else if (take == "TUTORIAL") {
        numTut = numTut + 1;
    }
}

//check if there are enough resources to make trade
bool Student::verifyEnoughResources(string typeOfResource) {
    if ((typeOfResource == "CAFFEIENE") && (numCaf >= 1)) return true;
    else if ((typeOfResource == "STUDY") && (numStu >= 1)) return true;
    else if ((typeOfResource == "LAB") && (numLab >= 1)) return true;
    else if ((typeOfResource == "LECTURE") && (numLec >= 1)) return true;
    else if ((typeOfResource == "TUTORIAL") && (numTut >= 1)) return true;
    else return false;
}

//performing the trade bewteeing *other and this
void Student::tradeResources(Student *other, string give, string take){
    if ((this->verifyEnoughResources(give) == true) &&
        (other->verifyEnoughResources(take) == true)) {
        this->receiveTrade(give, take);
        other->receiveTrade(take, give);
    } else {
        throw string("You do not have enough resources.");
    }
}


//best function
std::string Student::getStudentColour(){
    return studentColour;
}


//the next many functions deal with whether there are enough resources to build things
// and taking away resources if the student wants to build that criteria/goal
bool Student::validGoal() {
    if ((numStu >= 1) && (numTut >= 1)) {
        return true;
    } else {
        return false;
    }
}


void Student::buildGoal(bool isBegin, int pos){
    if (isBegin == false) {
        numStu = numStu - 1;
        numTut = numTut - 1;
    }
    goalsOwned.push_back(pos);
}


bool Student::validAssignment() {
    if ((numCaf >= 1) && (numLab >= 1) &&
        (numLec >= 1) && (numTut >= 1)) {
        return true;
    } else {
        return false;
    }
}


bool Student::validMidterm() {
    if ((numLec >= 2) && (numStu >= 3)) {
        return true;
    } else {
        return false;
    }
}


bool Student::validExam() {
    if ((numCaf >= 3) && (numLab >= 2) &&
        (numLec >= 2) && (numTut >= 1) &&
        (numStu >= 2)) {
        return true;
    } else {
        return false;
    }
}


void Student::buildAssignment(bool isBegin, int pos) {
    if (isBegin == false) {
        numCaf = numCaf - 1;
        numLab = numLab - 1;
        numLec = numLec - 1;
        numTut = numTut - 1;
    }
    victoryPoints = victoryPoints + 1;
    criterionOwned.push_back(pos);
    criteriaType.push_back(1);
}



void Student::buildMidterm(bool isBegin, int pos) {
    if (isBegin == false) {
        numLec = numLec - 2;
        numStu = numStu - 3;
    }
    victoryPoints = victoryPoints + 1;

    int upgradePos = 0;
    for (int i = 0; i < criterionOwned.size(); i++){
	if (criterionOwned[i] == pos){
	    upgradePos = i;
	}	
    }
    criteriaType[upgradePos] = 2;
}



void Student::buildExam(bool isBegin, int pos) {
    if (isBegin == false) {
	numCaf = numCaf - 3;
        numLab = numLab - 2;
        numLec = numLec - 2;
        numTut = numTut - 1;
        numStu = numStu - 2;
    }
    victoryPoints = victoryPoints + 1;
	int upgradePos = 0;
    for (int i = 0; i < criterionOwned.size(); i++){
        if (criterionOwned[i] == pos){
            upgradePos = i;
        }
    }
    criteriaType[upgradePos] = 2;
}



//when student's criteria is notified, we add resources for the student
void Student::notifyStudent(string typeOfResource, int howMany) {
    if (typeOfResource == "CAFFEINE") {
        numCaf = numCaf + howMany;
	tempResources["CAFFEINE"] += howMany;
    } else if (typeOfResource == "STUDY") {
        numStu = numStu + howMany;
	tempResources["STUDY"] += howMany;
    } else if (typeOfResource == "LAB") {
        numLab = numLab + howMany;
	tempResources["LAB"] += howMany;
    } else if (typeOfResource == "LECTURE") {
        numLec = numLec + howMany;
	tempResources["LECTURE"] += howMany;
    } else if (typeOfResource == "TUTORIAL") {
        numTut = numTut + howMany;
	tempResources["TUTORIAL"] += howMany;
    }
    
    
}


//prints what the student managed to collect this die roll
int Student::displayResourceIncome(){
    if (tempResources.size() == 0){
	return 0;
    }
    cout << "Student " << this->getStudentColour() << " gained:" << endl;
    for (auto x: tempResources) {
	cout << "    "  << x.second << " " << x.first << endl;
    }	
    tempResources.erase(tempResources.begin(), tempResources.end());
    return 1;
}


//based on the ratio of resources to total, u lose half if u have more than
// 10 resources and 7 is rolled
void Student::loseResources(){
    int total = numCaf + numLab + numLec + numStu + numTut;
    
    if (total >= 10){
        int lost = total / 2;
        int cafLost = lost * numCaf / total;
        int labLost = lost * numStu / total;
        int lecLost = lost * numLec / total;
        int stuLost = lost * numStu / total;
        int tutLost = lost * numTut / total;
        map<string, int> lostMap;
        lostMap["CAFFEINE"] = cafLost;
        lostMap["LAB"] = labLost;
        lostMap["LECTURE"] = lecLost;
        lostMap["STUDY"] = stuLost;
        lostMap["TUTORIAL"] = tutLost;
        numCaf -= cafLost;
        numLab -= labLost;
        numLec -= lecLost;
        numStu -= stuLost;
        numTut -= tutLost;
        
        //print it out!
        cout << "Student " << studentColour << " loses " << lost << " resources to the geese. They lose:" << endl;
        for (auto &p : lostMap){
            cout << p.second << " " << p.first <<endl;
        }
        
    }
}


//stealing function, the theif gets the resource which is random
void Student::gainFromSteal(string resource){
    if (resource == "CAFFEINE"){
	numCaf++;
    } else if (resource == "LAB"){
	numLab++;
    } else if (resource == "LECTURE"){
	numLec++;
    } else if (resource == "STUDY"){
	numStu++;
    } else if (resource == "TUTORIAL"){
	numTut++;
    }
}

void Student::loseFromSteal(Student *theif){
    map<string, int*> lostMap;
    lostMap["CAFFEINE"] = &numCaf;
    lostMap["LAB"] = &numLab;
    lostMap["LECTURE"] = &numLec;
    lostMap["STUDY"] = &numStu;
    lostMap["TUTORIAL"] = &numTut;
    //the resource student has most is the one he loses. 
    int maxVal = 0;
    string stealResc = "";
    for (auto x: lostMap){
        if (*x.second > maxVal){
	    stealResc = x.first;
            maxVal = *x.second;
	}
    } 
    if (maxVal == 0){
	cout << "There are no resources to steal from " << this->studentColour << endl;
    } else {
        cout << theif->getStudentColour() << " steals " << stealResc << " from " << this->studentColour << endl;
        int *p = lostMap.at(stealResc);
	*p = *p - 1;
        theif->gainFromSteal(stealResc);
    }     
}

//fair or loaded?
void Student::setTypeOfDice(string type) {
    this->typeOfDice = type;
}

int Student::studentRoll() {
    if (typeOfDice == "fair") {
        return fairDice->rollDie();
    } else {
        return loadedDice->rollDie();
    }
}


//print out criteria owned, goals owned, and status
void Student::printCriteria() { 
    cout << studentColour << " has completed:" << endl;
    for(int i = 0; i < criterionOwned.size(); ++i) {
        cout << criterionOwned[i] << " " << criteriaType[i] << endl;
    }
}

void Student::printStatus(){
    cout << studentColour << " has " << victoryPoints << " victory points, ";
    cout << numCaf << " caffeines, " << numLab << " labs," << endl;
    cout << numLec << " lectures, " << numTut << " tutorials, and ";
    cout << numStu << " studies." << endl;
}






