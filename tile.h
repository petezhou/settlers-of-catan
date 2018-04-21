#ifndef TILE
#define TILE

#include <string>
#include <vector>
#include "subject.h"

class Criteria;
class Goal;
class Student;

class Tile : public Subject {
    bool isGeeseOver;
    int index;
    int val;
    std::string resource;
    
    Goal* goals[6];
    
public:
    Tile(int index, std::string resource, int val);
    ~Tile() override;
    
    int getVal();
    int getPos();
    std::string getName();

    void setGeese(bool g);
    Goal* getGoal(int idx);
    void setGoal(Goal* goal, int idx);
    
    std::vector<Student *> ownedCriterion(Student *theif); 

    void draw(char **c, int row, int col);
    std::string getInfo() override;   
    bool getGeeseOver();
};

#endif

