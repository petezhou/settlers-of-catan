#ifndef GOAL_H
#define GOAL_H

#include <vector>
#include "observer.h"
#include "subject.h"

class Student;
class Criteria;

class Goal {
    Student* owner;
    int index;
    std::vector<Goal *> neighGoals;
    std::vector<Criteria *> neighCriteria;
public:
    Goal(int pos);
    ~Goal();
    
    void buildGoal(Student *s, bool isBegin);
    
    void checkGoalNeighbours(bool *valid);
    
    void attachGoal(Goal *g);
    void attachCriteria(Criteria *c);
    
    Student *getOwner();
    bool ownedOrNot();    
    void draw(char ** canvas, int row, int col, int sequence) ;
    
    
};


#endif


