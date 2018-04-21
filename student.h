#ifndef STUDENT_H
#define STUDENT_H

#include <vector>
#include <string>
#include "factory.h"
#include <map>

class Criteria;
class Goal;
class Dice;
class Factory;

class Student{
    std::string studentColour;
    int victoryPoints;
    
    Factory f;
    std::string typeOfDice = "fair";
    Dice *fairDice = f.createDice("fair");
    Dice *loadedDice = f.createDice("loaded");
    
    std::vector<int> criterionOwned;
    std::vector<int> criteriaType;
    std::vector<int> goalsOwned;
    
    int numCaf;
    int numLab;
    int numLec;
    int numStu;
    int numTut;

    std::map<std::string, int> tempResources;
public:
    Student(std::string colour);

    ~Student();
    void resetStudent();
    
    void setResources(std::vector<int> resources);
    
    bool isWon();
    
    //for trading
    bool verifyEnoughResources(std::string typeOfResource);
    void tradeResources(Student *other, std::string give, std::string take);
    void receiveTrade(std::string give, std::string take);
	    
    //verifying if there are enough resources and taking away resource
    bool validGoal();
    void buildGoal(bool isBegin, int pos);
    bool validAssignment();
    void buildAssignment(bool isBegin, int pos);
    bool validMidterm();
    void buildMidterm(bool isBegin, int pos);
    bool validExam();
    void buildExam(bool isBegin, int pos);
    //for when a studen's criteria is notified
    void notifyStudent(std::string typeOfResource, int howMany);
    
    std::string getStudentColour();	
    int displayResourceIncome();
    //geese logic
    void loseResources();
    void gainFromSteal(std::string resource);
    void loseFromSteal(Student *theif);
    //rolling dice
    int studentRoll();
    void setTypeOfDice(std::string type);
    //displaying things logic
    void printCriteria();
    void printStatus();
    std::string printSave();
};


#endif

