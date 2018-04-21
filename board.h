#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <map>

//Define Tile, Goal and Achievement
class Tile;
class Goal;
class Criteria;
class TextDisplay;
class Student;

class Board{
    TextDisplay *td = nullptr;
    std::vector<Goal *> theGoals;
    std::vector<Tile *> theTiles;
    std::vector<Criteria *> theCriteria;

    //val(int), index(int), name(str) 
    

    std::map<int, int> mapTiles(); 
public:
    Tile *lastGeese = nullptr;
    ~Board();
    //functions used for initializing board
    void initBoard(std::vector<int> data);
    void initSeed(int seed);
    void attachAllNeighbours();    
    void displayBoard();
    //builidng objects onto our board such as criteria and goal
    void buildGoal(Student *s, int pos, bool isBegin);
    void buildCriteria(Student *s, int pos, bool isBegin);
    void improveCriteria(Student *s, int pos, bool isBegin);
    void setGeese(int pos, Student *thief);
    
    void yieldResources(int diceRoll);

    //getters and setters 	   
    std::vector<Tile *> getTiles();
    std::vector<Goal *> getGoals();
    std::vector<Criteria *> getCriterias();

    std::string printSave() ; 
    std::string getGeesePos();        
};


#endif

