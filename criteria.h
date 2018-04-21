#ifndef criteria_H
#define criteria_H

#include "observer.h"
#include "subject.h"

#include <string>


class Student;
class Goal;
class Criteria;

class Criteria : public Observer {
    bool ownedOrNot = false;
    Student *owner;
    std::string criteriaType;
    int index;
    std::vector<Goal*> neighGoal;
    std::vector<Criteria*> neighCriteria;
public:
    Criteria(int pos);
    ~Criteria() override;
   //notify from tyle 
    void notify(std::string typeOfResource) override;
    std::string getType();
    
    Student *getOwner();
    
   //actually making the goal/criteria
    void buildCriteria(Student *s, bool isBegin);
    void improveCriteria(Student *s, bool isBegin);
    
    //checking the neighbour logic
    void attachGoal(Goal *g);
    void attachCritera(Criteria *c);
    bool checkCriteriaNeighbours();
    bool checkOwnedOrNot();    

    void draw(char **canvas, int row, int col, int sequence);
    
};

#endif

