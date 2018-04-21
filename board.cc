#include "board.h"
#include "goal.h"
#include "criteria.h"
#include "textdisplay.h"
#include "tile.h"
#include "student.h"
#include "position.h"
#include "util.h"
#include <iostream>
#include "student.h"
#include <sstream>
#include <algorithm>

using namespace std;

//the destructor, deleting all of its components
Board::~Board(){
    delete td;
    //mapTiles.clear();        
    for(int i = 0; i < theGoals.size(); ++i) {
	delete theGoals[i];
    }
    for(int i = 0; i < theCriteria.size(); ++i) {
	delete theCriteria[i];
    }
    for(int i = 0; i < theTiles.size(); ++i) {
	delete theTiles[i];
    }
}

//get the position of the GEESE right now one the board
string Board::getGeesePos() {
    int pos = 0;
    for(int i = 0; i < theTiles.size(); ++i) {
        if ((theTiles[i])->getGeeseOver() == true) pos = i;
    }
    stringstream ss;
    ss << pos;
    string str = ss.str();
    return str;
}

//translating from the numbers in load file to actual resources
std::string translateResource(int n){
    if(n == 0){
        return "CAFFEINE";
    } else if (n == 1) {
        return "LAB";
    } else if (n == 2) {
        return "LECTURE";
    } else if (n == 3){
        return "STUDY";
    } else if (n == 4){
        return "TUTORIAL";
    } else if (n == 5){
        return "NETFLIX";
    } else {
        return "BUG";
    }
}


//the main initialization of all our things
void Board::initBoard(vector<int> data){
    
    //CONST
    const int TOTALTILENUM = 19;
    const int TOTALVERTEXNUM = 54;
    const int TOTALEDGENUM = 72;
    
    int pos = 0;
    //crate all tiles
    for (int i = 0; i < TOTALTILENUM; i++) {
        string resc = translateResource(data[pos]);
        int val = data[pos + 1];
        theTiles.emplace_back(new Tile(i, resc, val));
	if (theTiles[i]->getVal() == 7){
	    Tile *curGeese = theTiles[i];
            curGeese->setGeese(true);
            lastGeese = curGeese;
        }
        pos += 2;
    }
    
    //create all courses
    for (int i = 0; i < TOTALVERTEXNUM; i++) {
        theCriteria.emplace_back(new Criteria(i));
    }
    
    //create all goals
    for (int i = 0; i < TOTALEDGENUM; i++) {
        theGoals.emplace_back(new Goal(i));
    }
    
    //assign criterion/goals
    for (int i = 0; i < 19; i++) {
        Position pos = Util::getTilePosition(i);
        
        int* coursesAssigned = Util::getTileVertexDistribution(i);
        for (int idx = 0; idx < 6; idx++) {
            theTiles[i]->attach(theCriteria[coursesAssigned[idx]]);
        }
        
        int* goalsAssigned = Util::getTileEdgeDistribution(i);
        for (int idx = 0; idx < 6; idx ++) {
            theTiles[i]->setGoal(theGoals[goalsAssigned[idx]], idx);
        }
    }
    this->attachAllNeighbours();   
    //send it to text display to print to screen
    td = new TextDisplay(theTiles);
}





//call the TextDisplay class to print the board
void Board::displayBoard(){
    cout << *td << endl;
}

//call set goal.buildGoal at its postion
void Board::buildGoal(Student *s, int pos, bool isBegin){
    if ((pos >= 0) && (pos <= 71)) {
        theGoals[pos]->buildGoal(s, isBegin);
    } else {
	throw string("You cannot build here.");
    }
}

//call criteria.buildCriteria
void Board::buildCriteria(Student *s, int pos, bool isBegin){
    if ((pos >= 0) && (pos <= 53)) {
	theCriteria[pos]->buildCriteria(s, isBegin);
    } else {
	throw string("You cannot build here.");
    }
}


void Board::improveCriteria(Student *s, int pos, bool isBegin){
    if ((pos >= 0) && (pos <= 53)) {
        theCriteria[pos]->improveCriteria(s, isBegin);
    } else {
	throw string("Invalid criteria to improve.");
    }
}


//set geese logic for stealing resources
void Board::setGeese(int pos, Student *theif){
    if (lastGeese != nullptr){
        lastGeese->setGeese(false);
    }
    Tile *curGeese = theTiles[pos];
    curGeese->setGeese(true);
    lastGeese = curGeese;
    //return all owners of criteria surrounding that tile
    vector<Student *> owners = curGeese->ownedCriterion(theif);
    //just print out the options    
    if (owners.size() != 0){
        cout << "Student " << theif->getStudentColour() << " can choose to steal from ";
        for (int i = 0; i < owners.size(); i++){
            cout << owners[i]->getStudentColour() << " ";
        }
        cout << endl;
    } else {
        cout << "Student " << theif->getStudentColour() << " has no one to steal from" << endl;
        return;
    }
    
    string victim;
    cout << ">";
    cin >> victim;
    transform(victim.begin(), victim.end(), victim.begin(), ::toupper);
    
    for (int i = 0; i < owners.size(); i++){
        if(victim == owners[i]->getStudentColour()){
            //call a steal function
            owners[i]->loseFromSteal(theif);
        }
    }
    
}

//calls the notifyObservers() for our main observer pattern
void Board::yieldResources(int diceRoll){
    for (int i = 0; i < 19; i++){
        Tile *curTile = theTiles[i];
        if (diceRoll == curTile->getVal() && !curTile->getGeeseOver()) {
            curTile->notifyObservers();
        }
    }
}

//gets the tile pointers
vector<Tile *> Board::getTiles(){
    return this->theTiles;
}

//map tiles to numbers for SAVE, similar to load but the other way
map<int, int> Board::mapTiles() {
    map<int, int> tileMap ; 

    for(int i = 0; i < theTiles.size(); i++) {
        int res ; 
        string resName = theTiles[i]->getName() ; 

        if(resName == "CAFFEINE") {
            res = 0 ;
        } else if(resName == "LAB") {
            res = 1 ;
        } else if(resName == "LECTURE") {
            res = 2 ;
        } else if(resName == "STUDY") {
            res = 3 ;
        } else if(resName == "TUTORIAL") {
            res = 4 ;
        } else {
            res = 5 ;
        }

        tileMap[theTiles[i]->getVal()] = res; 

    }

    return tileMap ; 
}

//print board data to the save.txt file.
string Board::printSave() {
    //resource then value 

    string tileStr = "" ; 
    map<int, int> theMap = mapTiles() ; 

    for(int i = 0; i < theTiles.size() ; i++) {
	string resName = theTiles[i]->getName() ;       
        stringstream ss; 
        stringstream  tt;

	int res; 

	if(resName == "CAFFEINE") {
            res = 0 ;
        } else if(resName == "LAB") {
            res = 1 ;
        } else if(resName == "LECTURE") {
            res = 2 ;
        } else if(resName == "STUDY") {
            res = 3 ;
        } else if(resName == "TUTORIAL") {
            res = 4 ;
        } else  {
            res = 5 ;
        }
	
	string s ;
        ss << res ;
        tt << theTiles.at(i)->getVal() ;
        s = ss.str() + " " + tt.str() + " " ;

        tileStr += s ;
    }
    return tileStr ; 

}



//attaching neighbours to goals and criteria because we need to check if its a legal placement
void Board::attachAllNeighbours() {
    // Attaching goals to goals
    ((theGoals)[0])->attachGoal(theGoals[1]);
    ((theGoals)[0])->attachGoal(theGoals[2]);
    ((theGoals)[1])->attachGoal(theGoals[0]);
    ((theGoals)[1])->attachGoal(theGoals[3]);
    ((theGoals)[1])->attachGoal(theGoals[6]);
    ((theGoals)[2])->attachGoal(theGoals[0]);
    ((theGoals)[2])->attachGoal(theGoals[4]);
    ((theGoals)[2])->attachGoal(theGoals[7]);
    ((theGoals)[3])->attachGoal(theGoals[1]);
    ((theGoals)[3])->attachGoal(theGoals[5]);
    ((theGoals)[3])->attachGoal(theGoals[6]);
    ((theGoals)[4])->attachGoal(theGoals[2]);
    ((theGoals)[4])->attachGoal(theGoals[7]);
    ((theGoals)[4])->attachGoal(theGoals[8]);
    ((theGoals)[5])->attachGoal(theGoals[3]);
    ((theGoals)[5])->attachGoal(theGoals[9]);
    ((theGoals)[5])->attachGoal(theGoals[13]);
    ((theGoals)[6])->attachGoal(theGoals[1]);
    ((theGoals)[6])->attachGoal(theGoals[3]);
    ((theGoals)[6])->attachGoal(theGoals[10]);
    ((theGoals)[6])->attachGoal(theGoals[14]);
    ((theGoals)[7])->attachGoal(theGoals[2]);
    ((theGoals)[7])->attachGoal(theGoals[4]);
    ((theGoals)[7])->attachGoal(theGoals[10]);
    ((theGoals)[7])->attachGoal(theGoals[15]);
    ((theGoals)[8])->attachGoal(theGoals[4]);
    ((theGoals)[8])->attachGoal(theGoals[11]);
    ((theGoals)[8])->attachGoal(theGoals[16]);
    ((theGoals)[9])->attachGoal(theGoals[5]);
    ((theGoals)[9])->attachGoal(theGoals[12]);
    ((theGoals)[9])->attachGoal(theGoals[13]);
    ((theGoals)[10])->attachGoal(theGoals[6]);
    ((theGoals)[10])->attachGoal(theGoals[7]);
    ((theGoals)[10])->attachGoal(theGoals[14]);
    ((theGoals)[10])->attachGoal(theGoals[15]);
    ((theGoals)[11])->attachGoal(theGoals[8]);
    ((theGoals)[11])->attachGoal(theGoals[16]);
    ((theGoals)[11])->attachGoal(theGoals[17]);
    ((theGoals)[12])->attachGoal(theGoals[9]);
    ((theGoals)[12])->attachGoal(theGoals[20]);
    ((theGoals)[13])->attachGoal(theGoals[5]);
    ((theGoals)[13])->attachGoal(theGoals[9]);
    ((theGoals)[13])->attachGoal(theGoals[18]);
    ((theGoals)[13])->attachGoal(theGoals[21]);
    ((theGoals)[14])->attachGoal(theGoals[6]);
    ((theGoals)[14])->attachGoal(theGoals[10]);
    ((theGoals)[14])->attachGoal(theGoals[18]);
    ((theGoals)[14])->attachGoal(theGoals[22]);
    ((theGoals)[15])->attachGoal(theGoals[7]);
    ((theGoals)[15])->attachGoal(theGoals[10]);
    ((theGoals)[15])->attachGoal(theGoals[19]);
    ((theGoals)[15])->attachGoal(theGoals[23]);
    ((theGoals)[16])->attachGoal(theGoals[8]);
    ((theGoals)[16])->attachGoal(theGoals[11]);
    ((theGoals)[16])->attachGoal(theGoals[19]);
    ((theGoals)[16])->attachGoal(theGoals[24]);
    ((theGoals)[17])->attachGoal(theGoals[11]);
    ((theGoals)[17])->attachGoal(theGoals[25]);
    ((theGoals)[18])->attachGoal(theGoals[13]);
    ((theGoals)[18])->attachGoal(theGoals[14]);
    ((theGoals)[18])->attachGoal(theGoals[21]);
    ((theGoals)[18])->attachGoal(theGoals[22]);
    ((theGoals)[19])->attachGoal(theGoals[15]);
    ((theGoals)[19])->attachGoal(theGoals[16]);
    ((theGoals)[19])->attachGoal(theGoals[23]);
    ((theGoals)[19])->attachGoal(theGoals[24]);
    ((theGoals)[20])->attachGoal(theGoals[12]);
    ((theGoals)[20])->attachGoal(theGoals[26]);
    ((theGoals)[20])->attachGoal(theGoals[29]);
    ((theGoals)[21])->attachGoal(theGoals[13]);
    ((theGoals)[21])->attachGoal(theGoals[18]);
    ((theGoals)[21])->attachGoal(theGoals[26]);
    ((theGoals)[21])->attachGoal(theGoals[30]);
    ((theGoals)[22])->attachGoal(theGoals[14]);
    ((theGoals)[22])->attachGoal(theGoals[18]);
    ((theGoals)[22])->attachGoal(theGoals[27]);
    ((theGoals)[22])->attachGoal(theGoals[31]);
    ((theGoals)[23])->attachGoal(theGoals[15]);
    ((theGoals)[23])->attachGoal(theGoals[19]);
    ((theGoals)[23])->attachGoal(theGoals[27]);
    ((theGoals)[23])->attachGoal(theGoals[32]);
    ((theGoals)[24])->attachGoal(theGoals[16]);
    ((theGoals)[24])->attachGoal(theGoals[19]);
    ((theGoals)[24])->attachGoal(theGoals[23]);
    ((theGoals)[24])->attachGoal(theGoals[28]);
    ((theGoals)[25])->attachGoal(theGoals[17]);
    ((theGoals)[25])->attachGoal(theGoals[28]);
    ((theGoals)[25])->attachGoal(theGoals[34]);
    ((theGoals)[26])->attachGoal(theGoals[20]);
    ((theGoals)[26])->attachGoal(theGoals[21]);
    ((theGoals)[26])->attachGoal(theGoals[29]);
    ((theGoals)[26])->attachGoal(theGoals[30]);
    ((theGoals)[27])->attachGoal(theGoals[22]);
    ((theGoals)[27])->attachGoal(theGoals[23]);
    ((theGoals)[27])->attachGoal(theGoals[31]);
    ((theGoals)[27])->attachGoal(theGoals[32]);
    ((theGoals)[28])->attachGoal(theGoals[24]);
    ((theGoals)[28])->attachGoal(theGoals[25]);
    ((theGoals)[28])->attachGoal(theGoals[33]);
    ((theGoals)[28])->attachGoal(theGoals[34]);
    ((theGoals)[29])->attachGoal(theGoals[20]);
    ((theGoals)[29])->attachGoal(theGoals[26]);
    ((theGoals)[29])->attachGoal(theGoals[37]);
    ((theGoals)[30])->attachGoal(theGoals[21]);
    ((theGoals)[30])->attachGoal(theGoals[26]);
    ((theGoals)[30])->attachGoal(theGoals[35]);
    ((theGoals)[30])->attachGoal(theGoals[38]);
    ((theGoals)[31])->attachGoal(theGoals[22]);
    ((theGoals)[31])->attachGoal(theGoals[27]);
    ((theGoals)[31])->attachGoal(theGoals[35]);
    ((theGoals)[31])->attachGoal(theGoals[39]);
    ((theGoals)[32])->attachGoal(theGoals[23]);
    ((theGoals)[32])->attachGoal(theGoals[27]);
    ((theGoals)[32])->attachGoal(theGoals[36]);
    ((theGoals)[32])->attachGoal(theGoals[40]);
    ((theGoals)[33])->attachGoal(theGoals[24]);
    ((theGoals)[33])->attachGoal(theGoals[26]);
    ((theGoals)[33])->attachGoal(theGoals[28]);
    ((theGoals)[33])->attachGoal(theGoals[41]);
    ((theGoals)[34])->attachGoal(theGoals[25]);
    ((theGoals)[34])->attachGoal(theGoals[28]);
    ((theGoals)[34])->attachGoal(theGoals[42]);
    ((theGoals)[35])->attachGoal(theGoals[30]);
    ((theGoals)[35])->attachGoal(theGoals[31]);
    ((theGoals)[35])->attachGoal(theGoals[38]);
    ((theGoals)[35])->attachGoal(theGoals[39]);
    ((theGoals)[36])->attachGoal(theGoals[32]);
    ((theGoals)[36])->attachGoal(theGoals[33]);
    ((theGoals)[36])->attachGoal(theGoals[40]);
    ((theGoals)[36])->attachGoal(theGoals[41]);
    ((theGoals)[37])->attachGoal(theGoals[29]);
    ((theGoals)[37])->attachGoal(theGoals[43]);
    ((theGoals)[37])->attachGoal(theGoals[46]);
    ((theGoals)[38])->attachGoal(theGoals[30]);
    ((theGoals)[38])->attachGoal(theGoals[35]);
    ((theGoals)[38])->attachGoal(theGoals[38]);
    ((theGoals)[39])->attachGoal(theGoals[31]);
    ((theGoals)[39])->attachGoal(theGoals[35]);
    ((theGoals)[39])->attachGoal(theGoals[44]);
    ((theGoals)[39])->attachGoal(theGoals[48]);
    ((theGoals)[40])->attachGoal(theGoals[32]);
    ((theGoals)[40])->attachGoal(theGoals[36]);
    ((theGoals)[40])->attachGoal(theGoals[44]);
    ((theGoals)[40])->attachGoal(theGoals[49]);
    ((theGoals)[41])->attachGoal(theGoals[33]);
    ((theGoals)[41])->attachGoal(theGoals[36]);
    ((theGoals)[41])->attachGoal(theGoals[45]);
    ((theGoals)[41])->attachGoal(theGoals[50]);
    ((theGoals)[42])->attachGoal(theGoals[34]);
    ((theGoals)[42])->attachGoal(theGoals[45]);
    ((theGoals)[42])->attachGoal(theGoals[51]);
    ((theGoals)[43])->attachGoal(theGoals[37]);
    ((theGoals)[43])->attachGoal(theGoals[38]);
    ((theGoals)[43])->attachGoal(theGoals[46]);
    ((theGoals)[43])->attachGoal(theGoals[47]);
    ((theGoals)[44])->attachGoal(theGoals[39]);
    ((theGoals)[44])->attachGoal(theGoals[40]); 
    ((theGoals)[44])->attachGoal(theGoals[48]);
    ((theGoals)[44])->attachGoal(theGoals[49]);
    ((theGoals)[45])->attachGoal(theGoals[41]);    
    ((theGoals)[45])->attachGoal(theGoals[42]);
    ((theGoals)[45])->attachGoal(theGoals[50]);
    ((theGoals)[45])->attachGoal(theGoals[51]);
    ((theGoals)[46])->attachGoal(theGoals[37]);
    ((theGoals)[46])->attachGoal(theGoals[43]);
    ((theGoals)[46])->attachGoal(theGoals[54]);
    ((theGoals)[47])->attachGoal(theGoals[38]);
    ((theGoals)[47])->attachGoal(theGoals[43]);
    ((theGoals)[47])->attachGoal(theGoals[52]);
    ((theGoals)[47])->attachGoal(theGoals[55]);
    ((theGoals)[48])->attachGoal(theGoals[39]);
    ((theGoals)[48])->attachGoal(theGoals[44]);
    ((theGoals)[48])->attachGoal(theGoals[52]);
    ((theGoals)[48])->attachGoal(theGoals[56]);
    ((theGoals)[49])->attachGoal(theGoals[40]);
    ((theGoals)[49])->attachGoal(theGoals[44]);
    ((theGoals)[49])->attachGoal(theGoals[53]);
    ((theGoals)[49])->attachGoal(theGoals[57]);
    ((theGoals)[50])->attachGoal(theGoals[41]);
    ((theGoals)[50])->attachGoal(theGoals[45]);
    ((theGoals)[50])->attachGoal(theGoals[53]);
    ((theGoals)[50])->attachGoal(theGoals[58]);
    ((theGoals)[51])->attachGoal(theGoals[42]);
    ((theGoals)[51])->attachGoal(theGoals[45]);
    ((theGoals)[51])->attachGoal(theGoals[59]);
    ((theGoals)[52])->attachGoal(theGoals[47]);
    ((theGoals)[52])->attachGoal(theGoals[48]);
    ((theGoals)[52])->attachGoal(theGoals[55]);
    ((theGoals)[52])->attachGoal(theGoals[56]);
    ((theGoals)[53])->attachGoal(theGoals[49]);
    ((theGoals)[53])->attachGoal(theGoals[50]);
    ((theGoals)[53])->attachGoal(theGoals[57]);
    ((theGoals)[53])->attachGoal(theGoals[58]);
    ((theGoals)[54])->attachGoal(theGoals[46]);
    ((theGoals)[54])->attachGoal(theGoals[60]);
    ((theGoals)[55])->attachGoal(theGoals[47]);
    ((theGoals)[55])->attachGoal(theGoals[48]);
    ((theGoals)[55])->attachGoal(theGoals[55]);
    ((theGoals)[55])->attachGoal(theGoals[56]);
    ((theGoals)[56])->attachGoal(theGoals[48]);
    ((theGoals)[56])->attachGoal(theGoals[52]);
    ((theGoals)[56])->attachGoal(theGoals[61]);
    ((theGoals)[56])->attachGoal(theGoals[64]);
    ((theGoals)[57])->attachGoal(theGoals[49]);
    ((theGoals)[57])->attachGoal(theGoals[53]);
    ((theGoals)[57])->attachGoal(theGoals[61]);
    ((theGoals)[57])->attachGoal(theGoals[65]);
    ((theGoals)[58])->attachGoal(theGoals[50]);
    ((theGoals)[58])->attachGoal(theGoals[53]);
    ((theGoals)[58])->attachGoal(theGoals[62]);
    ((theGoals)[58])->attachGoal(theGoals[66]);
    ((theGoals)[59])->attachGoal(theGoals[51]);
    ((theGoals)[59])->attachGoal(theGoals[62]);
    ((theGoals)[60])->attachGoal(theGoals[54]);
    ((theGoals)[60])->attachGoal(theGoals[55]);
    ((theGoals)[60])->attachGoal(theGoals[63]);
    ((theGoals)[61])->attachGoal(theGoals[56]);
    ((theGoals)[61])->attachGoal(theGoals[57]);
    ((theGoals)[61])->attachGoal(theGoals[64]);
    ((theGoals)[61])->attachGoal(theGoals[65]);
    ((theGoals)[62])->attachGoal(theGoals[58]);
    ((theGoals)[62])->attachGoal(theGoals[59]);
    ((theGoals)[62])->attachGoal(theGoals[66]);
    ((theGoals)[63])->attachGoal(theGoals[55]);
    ((theGoals)[63])->attachGoal(theGoals[60]);
    ((theGoals)[63])->attachGoal(theGoals[67]);
    ((theGoals)[64])->attachGoal(theGoals[56]);
    ((theGoals)[64])->attachGoal(theGoals[61]);
    ((theGoals)[64])->attachGoal(theGoals[67]);
    ((theGoals)[64])->attachGoal(theGoals[69]);
    ((theGoals)[65])->attachGoal(theGoals[57]);
    ((theGoals)[65])->attachGoal(theGoals[61]);
    ((theGoals)[65])->attachGoal(theGoals[68]);
    ((theGoals)[65])->attachGoal(theGoals[70]);
    ((theGoals)[66])->attachGoal(theGoals[58]);
    ((theGoals)[66])->attachGoal(theGoals[62]);
    ((theGoals)[66])->attachGoal(theGoals[68]);
    ((theGoals)[67])->attachGoal(theGoals[63]);
    ((theGoals)[67])->attachGoal(theGoals[64]);
    ((theGoals)[67])->attachGoal(theGoals[69]);
    ((theGoals)[68])->attachGoal(theGoals[65]);
    ((theGoals)[68])->attachGoal(theGoals[66]);
    ((theGoals)[68])->attachGoal(theGoals[70]);
    ((theGoals)[69])->attachGoal(theGoals[64]);
    ((theGoals)[69])->attachGoal(theGoals[67]);
    ((theGoals)[69])->attachGoal(theGoals[71]);
    ((theGoals)[70])->attachGoal(theGoals[65]);
    ((theGoals)[70])->attachGoal(theGoals[68]);
    ((theGoals)[70])->attachGoal(theGoals[71]);
    ((theGoals)[71])->attachGoal(theGoals[69]);
    ((theGoals)[71])->attachGoal(theGoals[70]);

    // Attaching goals to criteria
    ((theGoals)[0])->attachCriteria(theCriteria[0]);
    ((theGoals)[0])->attachCriteria(theCriteria[1]);
    ((theGoals)[1])->attachCriteria(theCriteria[0]);
    ((theGoals)[1])->attachCriteria(theCriteria[3]);
    ((theGoals)[2])->attachCriteria(theCriteria[1]);
    ((theGoals)[2])->attachCriteria(theCriteria[4]);
    ((theGoals)[3])->attachCriteria(theCriteria[2]);
    ((theGoals)[3])->attachCriteria(theCriteria[3]);
    ((theGoals)[4])->attachCriteria(theCriteria[4]);
    ((theGoals)[4])->attachCriteria(theCriteria[5]);
    ((theGoals)[5])->attachCriteria(theCriteria[2]);
    ((theGoals)[5])->attachCriteria(theCriteria[7]);
    ((theGoals)[6])->attachCriteria(theCriteria[3]);
    ((theGoals)[6])->attachCriteria(theCriteria[8]);
    ((theGoals)[7])->attachCriteria(theCriteria[4]);
    ((theGoals)[7])->attachCriteria(theCriteria[9]);
    ((theGoals)[8])->attachCriteria(theCriteria[5]);
    ((theGoals)[8])->attachCriteria(theCriteria[10]);
    ((theGoals)[9])->attachCriteria(theCriteria[6]);
    ((theGoals)[9])->attachCriteria(theCriteria[7]);
    ((theGoals)[10])->attachCriteria(theCriteria[8]);
    ((theGoals)[10])->attachCriteria(theCriteria[9]);
    ((theGoals)[11])->attachCriteria(theCriteria[10]);
    ((theGoals)[11])->attachCriteria(theCriteria[11]);
    ((theGoals)[12])->attachCriteria(theCriteria[6]);
    ((theGoals)[12])->attachCriteria(theCriteria[12]);
    ((theGoals)[13])->attachCriteria(theCriteria[7]);
    ((theGoals)[13])->attachCriteria(theCriteria[13]);
    ((theGoals)[14])->attachCriteria(theCriteria[8]);
    ((theGoals)[14])->attachCriteria(theCriteria[14]);
    ((theGoals)[15])->attachCriteria(theCriteria[9]);
    ((theGoals)[15])->attachCriteria(theCriteria[15]);
    ((theGoals)[16])->attachCriteria(theCriteria[10]);
    ((theGoals)[16])->attachCriteria(theCriteria[16]);
    ((theGoals)[17])->attachCriteria(theCriteria[11]);
    ((theGoals)[17])->attachCriteria(theCriteria[17]);
    ((theGoals)[18])->attachCriteria(theCriteria[13]);
    ((theGoals)[18])->attachCriteria(theCriteria[14]);
    ((theGoals)[19])->attachCriteria(theCriteria[15]);
    ((theGoals)[19])->attachCriteria(theCriteria[16]);
    ((theGoals)[20])->attachCriteria(theCriteria[12]);
    ((theGoals)[20])->attachCriteria(theCriteria[18]);
    ((theGoals)[21])->attachCriteria(theCriteria[13]);
    ((theGoals)[21])->attachCriteria(theCriteria[19]);
    ((theGoals)[22])->attachCriteria(theCriteria[14]);
    ((theGoals)[22])->attachCriteria(theCriteria[20]);
    ((theGoals)[23])->attachCriteria(theCriteria[15]);
    ((theGoals)[23])->attachCriteria(theCriteria[21]);
    ((theGoals)[24])->attachCriteria(theCriteria[16]);
    ((theGoals)[24])->attachCriteria(theCriteria[22]);
    ((theGoals)[25])->attachCriteria(theCriteria[17]);
    ((theGoals)[25])->attachCriteria(theCriteria[23]);
    ((theGoals)[26])->attachCriteria(theCriteria[18]);
    ((theGoals)[26])->attachCriteria(theCriteria[19]);
    ((theGoals)[27])->attachCriteria(theCriteria[20]);
    ((theGoals)[27])->attachCriteria(theCriteria[21]);
    ((theGoals)[28])->attachCriteria(theCriteria[22]);
    ((theGoals)[28])->attachCriteria(theCriteria[23]);
    ((theGoals)[29])->attachCriteria(theCriteria[18]);
    ((theGoals)[29])->attachCriteria(theCriteria[24]); 
    ((theGoals)[30])->attachCriteria(theCriteria[19]);
    ((theGoals)[30])->attachCriteria(theCriteria[25]);
    ((theGoals)[31])->attachCriteria(theCriteria[20]);
    ((theGoals)[31])->attachCriteria(theCriteria[26]);
    ((theGoals)[32])->attachCriteria(theCriteria[21]);
    ((theGoals)[32])->attachCriteria(theCriteria[27]);
    ((theGoals)[33])->attachCriteria(theCriteria[22]);
    ((theGoals)[33])->attachCriteria(theCriteria[28]);
    ((theGoals)[34])->attachCriteria(theCriteria[23]);
    ((theGoals)[34])->attachCriteria(theCriteria[29]);
    ((theGoals)[35])->attachCriteria(theCriteria[25]);
    ((theGoals)[35])->attachCriteria(theCriteria[26]);
    ((theGoals)[36])->attachCriteria(theCriteria[27]);
    ((theGoals)[36])->attachCriteria(theCriteria[28]);
    ((theGoals)[37])->attachCriteria(theCriteria[24]);
    ((theGoals)[37])->attachCriteria(theCriteria[30]);
    ((theGoals)[38])->attachCriteria(theCriteria[25]);
    ((theGoals)[38])->attachCriteria(theCriteria[31]);
    ((theGoals)[39])->attachCriteria(theCriteria[26]);
    ((theGoals)[39])->attachCriteria(theCriteria[32]);
    ((theGoals)[40])->attachCriteria(theCriteria[27]);
    ((theGoals)[40])->attachCriteria(theCriteria[33]);
    ((theGoals)[41])->attachCriteria(theCriteria[28]);
    ((theGoals)[41])->attachCriteria(theCriteria[34]);
    ((theGoals)[42])->attachCriteria(theCriteria[29]);
    ((theGoals)[42])->attachCriteria(theCriteria[35]);   
    ((theGoals)[43])->attachCriteria(theCriteria[30]);
    ((theGoals)[43])->attachCriteria(theCriteria[31]);
    ((theGoals)[44])->attachCriteria(theCriteria[32]);
    ((theGoals)[44])->attachCriteria(theCriteria[33]);
    ((theGoals)[45])->attachCriteria(theCriteria[34]);
    ((theGoals)[45])->attachCriteria(theCriteria[35]);
    ((theGoals)[46])->attachCriteria(theCriteria[30]);
    ((theGoals)[46])->attachCriteria(theCriteria[36]);
    ((theGoals)[47])->attachCriteria(theCriteria[31]);
    ((theGoals)[47])->attachCriteria(theCriteria[37]);
    ((theGoals)[48])->attachCriteria(theCriteria[32]);
    ((theGoals)[48])->attachCriteria(theCriteria[38]);
    ((theGoals)[49])->attachCriteria(theCriteria[33]);
    ((theGoals)[49])->attachCriteria(theCriteria[39]);
    ((theGoals)[50])->attachCriteria(theCriteria[34]);
    ((theGoals)[50])->attachCriteria(theCriteria[40]);
    ((theGoals)[51])->attachCriteria(theCriteria[35]);
    ((theGoals)[51])->attachCriteria(theCriteria[41]);
    ((theGoals)[52])->attachCriteria(theCriteria[37]);
    ((theGoals)[52])->attachCriteria(theCriteria[38]);
    ((theGoals)[53])->attachCriteria(theCriteria[39]);
    ((theGoals)[53])->attachCriteria(theCriteria[40]);
    ((theGoals)[54])->attachCriteria(theCriteria[36]);
    ((theGoals)[54])->attachCriteria(theCriteria[42]);
    ((theGoals)[55])->attachCriteria(theCriteria[37]);
    ((theGoals)[55])->attachCriteria(theCriteria[43]);
    ((theGoals)[56])->attachCriteria(theCriteria[38]);
    ((theGoals)[56])->attachCriteria(theCriteria[44]);
    ((theGoals)[57])->attachCriteria(theCriteria[39]);
    ((theGoals)[57])->attachCriteria(theCriteria[45]);
    ((theGoals)[58])->attachCriteria(theCriteria[40]);
    ((theGoals)[58])->attachCriteria(theCriteria[46]);
    ((theGoals)[59])->attachCriteria(theCriteria[41]);
    ((theGoals)[59])->attachCriteria(theCriteria[47]);
    ((theGoals)[60])->attachCriteria(theCriteria[42]);
    ((theGoals)[60])->attachCriteria(theCriteria[43]);
    ((theGoals)[61])->attachCriteria(theCriteria[44]);
    ((theGoals)[61])->attachCriteria(theCriteria[45]);    
    ((theGoals)[62])->attachCriteria(theCriteria[46]);
    ((theGoals)[62])->attachCriteria(theCriteria[47]);
    ((theGoals)[63])->attachCriteria(theCriteria[43]);
    ((theGoals)[63])->attachCriteria(theCriteria[48]);
    ((theGoals)[64])->attachCriteria(theCriteria[44]);
    ((theGoals)[64])->attachCriteria(theCriteria[49]);
    ((theGoals)[65])->attachCriteria(theCriteria[45]);
    ((theGoals)[65])->attachCriteria(theCriteria[50]);
    ((theGoals)[66])->attachCriteria(theCriteria[46]);
    ((theGoals)[66])->attachCriteria(theCriteria[51]);
    ((theGoals)[67])->attachCriteria(theCriteria[48]);
    ((theGoals)[67])->attachCriteria(theCriteria[49]);
    ((theGoals)[68])->attachCriteria(theCriteria[50]);
    ((theGoals)[68])->attachCriteria(theCriteria[51]);
    ((theGoals)[69])->attachCriteria(theCriteria[49]);
    ((theGoals)[69])->attachCriteria(theCriteria[52]);
    ((theGoals)[70])->attachCriteria(theCriteria[50]);
    ((theGoals)[70])->attachCriteria(theCriteria[53]);
    ((theGoals)[71])->attachCriteria(theCriteria[52]);
    ((theGoals)[71])->attachCriteria(theCriteria[53]);

    // Attaching criteria to criteria
	((theCriteria)[0])->attachCritera(theCriteria[1]);
	((theCriteria)[0])->attachCritera(theCriteria[3]);

	((theCriteria)[1])->attachCritera(theCriteria[0]);
	((theCriteria)[1])->attachCritera(theCriteria[4]);

	((theCriteria)[2])->attachCritera(theCriteria[3]);
	((theCriteria)[2])->attachCritera(theCriteria[7]);

	((theCriteria)[3])->attachCritera(theCriteria[0]);
	((theCriteria)[3])->attachCritera(theCriteria[2]);
	((theCriteria)[3])->attachCritera(theCriteria[8]);

	((theCriteria)[4])->attachCritera(theCriteria[1]);
	((theCriteria)[4])->attachCritera(theCriteria[5]);
	((theCriteria)[4])->attachCritera(theCriteria[9]);

	((theCriteria)[5])->attachCritera(theCriteria[4]);
	((theCriteria)[5])->attachCritera(theCriteria[10]);

	((theCriteria)[6])->attachCritera(theCriteria[7]);
	((theCriteria)[6])->attachCritera(theCriteria[12]);

	((theCriteria)[7])->attachCritera(theCriteria[2]);
	((theCriteria)[7])->attachCritera(theCriteria[6]);
	((theCriteria)[7])->attachCritera(theCriteria[13]);

	((theCriteria)[8])->attachCritera(theCriteria[3]);
	((theCriteria)[8])->attachCritera(theCriteria[9]);
	((theCriteria)[8])->attachCritera(theCriteria[14]);

	((theCriteria)[9])->attachCritera(theCriteria[4]);
	((theCriteria)[9])->attachCritera(theCriteria[8]);
	((theCriteria)[9])->attachCritera(theCriteria[15]);

	((theCriteria)[10])->attachCritera(theCriteria[5]);
	((theCriteria)[10])->attachCritera(theCriteria[11]);
	((theCriteria)[10])->attachCritera(theCriteria[16]);

	((theCriteria)[11])->attachCritera(theCriteria[1]);
	((theCriteria)[11])->attachCritera(theCriteria[17]);

	((theCriteria)[12])->attachCritera(theCriteria[6]);
	((theCriteria)[12])->attachCritera(theCriteria[18]);

	((theCriteria)[13])->attachCritera(theCriteria[7]);
	((theCriteria)[13])->attachCritera(theCriteria[14]);
	((theCriteria)[13])->attachCritera(theCriteria[19]);

	((theCriteria)[14])->attachCritera(theCriteria[8]);
	((theCriteria)[14])->attachCritera(theCriteria[13]);
	((theCriteria)[14])->attachCritera(theCriteria[20]);

	((theCriteria)[15])->attachCritera(theCriteria[9]);
	((theCriteria)[15])->attachCritera(theCriteria[16]);
	((theCriteria)[15])->attachCritera(theCriteria[21]);

	((theCriteria)[16])->attachCritera(theCriteria[10]);
	((theCriteria)[16])->attachCritera(theCriteria[15]);
	((theCriteria)[16])->attachCritera(theCriteria[22]);

	((theCriteria)[17])->attachCritera(theCriteria[11]);
	((theCriteria)[17])->attachCritera(theCriteria[23]);

	((theCriteria)[18])->attachCritera(theCriteria[12]);
	((theCriteria)[18])->attachCritera(theCriteria[19]);
	((theCriteria)[18])->attachCritera(theCriteria[24]);

	((theCriteria)[19])->attachCritera(theCriteria[13]);
	((theCriteria)[19])->attachCritera(theCriteria[18]);
	((theCriteria)[19])->attachCritera(theCriteria[25]);

	((theCriteria)[20])->attachCritera(theCriteria[14]);
	((theCriteria)[20])->attachCritera(theCriteria[21]);
	((theCriteria)[20])->attachCritera(theCriteria[26]);

	((theCriteria)[21])->attachCritera(theCriteria[15]);
	((theCriteria)[21])->attachCritera(theCriteria[20]);
	((theCriteria)[21])->attachCritera(theCriteria[27]);

	((theCriteria)[22])->attachCritera(theCriteria[16]);
	((theCriteria)[22])->attachCritera(theCriteria[23]);
	((theCriteria)[22])->attachCritera(theCriteria[28]);

	((theCriteria)[23])->attachCritera(theCriteria[17]);
	((theCriteria)[23])->attachCritera(theCriteria[22]);
	((theCriteria)[23])->attachCritera(theCriteria[29]);

	((theCriteria)[24])->attachCritera(theCriteria[18]);
	((theCriteria)[24])->attachCritera(theCriteria[30]);

	((theCriteria)[25])->attachCritera(theCriteria[19]);
	((theCriteria)[25])->attachCritera(theCriteria[26]);
	((theCriteria)[25])->attachCritera(theCriteria[31]);

	((theCriteria)[26])->attachCritera(theCriteria[20]);
	((theCriteria)[26])->attachCritera(theCriteria[25]);
	((theCriteria)[26])->attachCritera(theCriteria[32]);

	((theCriteria)[27])->attachCritera(theCriteria[21]);
	((theCriteria)[27])->attachCritera(theCriteria[28]);
	((theCriteria)[27])->attachCritera(theCriteria[33]);

	((theCriteria)[28])->attachCritera(theCriteria[22]);
	((theCriteria)[28])->attachCritera(theCriteria[27]);
	((theCriteria)[28])->attachCritera(theCriteria[34]);

	((theCriteria)[29])->attachCritera(theCriteria[23]);
	((theCriteria)[29])->attachCritera(theCriteria[25]);

	((theCriteria)[30])->attachCritera(theCriteria[24]);
	((theCriteria)[30])->attachCritera(theCriteria[31]);
	((theCriteria)[30])->attachCritera(theCriteria[36]);

	((theCriteria)[31])->attachCritera(theCriteria[25]);
	((theCriteria)[31])->attachCritera(theCriteria[30]);
	((theCriteria)[31])->attachCritera(theCriteria[37]);

	((theCriteria)[32])->attachCritera(theCriteria[26]);
	((theCriteria)[32])->attachCritera(theCriteria[33]);
	((theCriteria)[32])->attachCritera(theCriteria[38]);

	((theCriteria)[33])->attachCritera(theCriteria[27]);
	((theCriteria)[33])->attachCritera(theCriteria[32]);
	((theCriteria)[33])->attachCritera(theCriteria[39]);

	((theCriteria)[34])->attachCritera(theCriteria[28]);
	((theCriteria)[34])->attachCritera(theCriteria[35]);
	((theCriteria)[34])->attachCritera(theCriteria[40]);

	((theCriteria)[35])->attachCritera(theCriteria[29]);
	((theCriteria)[35])->attachCritera(theCriteria[34]);
	((theCriteria)[35])->attachCritera(theCriteria[41]);

	((theCriteria)[36])->attachCritera(theCriteria[30]);
	((theCriteria)[36])->attachCritera(theCriteria[42]);

	((theCriteria)[37])->attachCritera(theCriteria[31]);
	((theCriteria)[37])->attachCritera(theCriteria[38]);
	((theCriteria)[37])->attachCritera(theCriteria[43]);

	((theCriteria)[38])->attachCritera(theCriteria[32]);
	((theCriteria)[38])->attachCritera(theCriteria[37]);
	((theCriteria)[38])->attachCritera(theCriteria[44]);

	((theCriteria)[39])->attachCritera(theCriteria[33]);
	((theCriteria)[39])->attachCritera(theCriteria[40]);
	((theCriteria)[39])->attachCritera(theCriteria[41]);

	((theCriteria)[40])->attachCritera(theCriteria[34]);
	((theCriteria)[40])->attachCritera(theCriteria[39]);
	((theCriteria)[40])->attachCritera(theCriteria[46]);

	((theCriteria)[41])->attachCritera(theCriteria[35]);
	((theCriteria)[41])->attachCritera(theCriteria[47]);

	((theCriteria)[42])->attachCritera(theCriteria[43]);
	((theCriteria)[42])->attachCritera(theCriteria[36]);

	((theCriteria)[43])->attachCritera(theCriteria[37]);
	((theCriteria)[43])->attachCritera(theCriteria[42]);
	((theCriteria)[43])->attachCritera(theCriteria[48]);

	((theCriteria)[44])->attachCritera(theCriteria[38]);
	((theCriteria)[44])->attachCritera(theCriteria[45]);
	((theCriteria)[44])->attachCritera(theCriteria[49]);

	((theCriteria)[45])->attachCritera(theCriteria[39]);
	((theCriteria)[45])->attachCritera(theCriteria[44]);
	((theCriteria)[45])->attachCritera(theCriteria[50]);

	((theCriteria)[46])->attachCritera(theCriteria[40]);
	((theCriteria)[46])->attachCritera(theCriteria[47]);
	((theCriteria)[46])->attachCritera(theCriteria[51]);

	((theCriteria)[47])->attachCritera(theCriteria[41]);
	((theCriteria)[47])->attachCritera(theCriteria[46]);

	((theCriteria)[48])->attachCritera(theCriteria[43]);
	((theCriteria)[48])->attachCritera(theCriteria[49]);

	((theCriteria)[49])->attachCritera(theCriteria[44]);
	((theCriteria)[49])->attachCritera(theCriteria[48]);
	((theCriteria)[49])->attachCritera(theCriteria[52]);

	((theCriteria)[50])->attachCritera(theCriteria[45]);
	((theCriteria)[50])->attachCritera(theCriteria[51]);
	((theCriteria)[50])->attachCritera(theCriteria[53]);

	((theCriteria)[51])->attachCritera(theCriteria[46]);
	((theCriteria)[51])->attachCritera(theCriteria[50]);

	((theCriteria)[52])->attachCritera(theCriteria[49]);
	((theCriteria)[52])->attachCritera(theCriteria[53]);

	((theCriteria)[53])->attachCritera(theCriteria[50]);
	((theCriteria)[53])->attachCritera(theCriteria[52]);

    // Attaching criteria to goals	
	((theCriteria)[0])->attachGoal(theGoals[0]);
	((theCriteria)[0])->attachGoal(theGoals[1]);
	
	((theCriteria)[1])->attachGoal(theGoals[0]);
	((theCriteria)[1])->attachGoal(theGoals[2]);

	((theCriteria)[2])->attachGoal(theGoals[3]);
	((theCriteria)[2])->attachGoal(theGoals[5]);

	((theCriteria)[3])->attachGoal(theGoals[1]);
	((theCriteria)[3])->attachGoal(theGoals[3]);
	((theCriteria)[3])->attachGoal(theGoals[6]);

	((theCriteria)[4])->attachGoal(theGoals[2]);
	((theCriteria)[4])->attachGoal(theGoals[4]);
	((theCriteria)[4])->attachGoal(theGoals[7]); 

	((theCriteria)[5])->attachGoal(theGoals[4]);
	((theCriteria)[5])->attachGoal(theGoals[8]);

	((theCriteria)[6])->attachGoal(theGoals[9]);
	((theCriteria)[6])->attachGoal(theGoals[12]);

	((theCriteria)[7])->attachGoal(theGoals[5]);
	((theCriteria)[7])->attachGoal(theGoals[9]);
	((theCriteria)[7])->attachGoal(theGoals[13]);

	((theCriteria)[8])->attachGoal(theGoals[6]);
	((theCriteria)[8])->attachGoal(theGoals[10]);
	((theCriteria)[8])->attachGoal(theGoals[14]);

	((theCriteria)[9])->attachGoal(theGoals[7]);
	((theCriteria)[9])->attachGoal(theGoals[10]);
	((theCriteria)[9])->attachGoal(theGoals[15]);

	((theCriteria)[10])->attachGoal(theGoals[8]);
	((theCriteria)[10])->attachGoal(theGoals[11]);
	((theCriteria)[10])->attachGoal(theGoals[16]);

	((theCriteria)[11])->attachGoal(theGoals[11]);
	((theCriteria)[11])->attachGoal(theGoals[17]);   

	((theCriteria)[12])->attachGoal(theGoals[12]);
	((theCriteria)[12])->attachGoal(theGoals[20]);

	((theCriteria)[13])->attachGoal(theGoals[13]);
	((theCriteria)[13])->attachGoal(theGoals[18]);
	((theCriteria)[13])->attachGoal(theGoals[21]);

	((theCriteria)[14])->attachGoal(theGoals[14]);
	((theCriteria)[14])->attachGoal(theGoals[18]);
	((theCriteria)[14])->attachGoal(theGoals[22]);

	((theCriteria)[15])->attachGoal(theGoals[15]);
	((theCriteria)[15])->attachGoal(theGoals[19]);
	((theCriteria)[15])->attachGoal(theGoals[23]);

	((theCriteria)[16])->attachGoal(theGoals[16]);
	((theCriteria)[16])->attachGoal(theGoals[19]);
	((theCriteria)[16])->attachGoal(theGoals[24]);

	((theCriteria)[17])->attachGoal(theGoals[17]);
	((theCriteria)[17])->attachGoal(theGoals[25]);

	((theCriteria)[18])->attachGoal(theGoals[20]);
	((theCriteria)[18])->attachGoal(theGoals[26]);
	((theCriteria)[18])->attachGoal(theGoals[29]);
	
	((theCriteria)[19])->attachGoal(theGoals[21]);
	((theCriteria)[19])->attachGoal(theGoals[26]);
	((theCriteria)[19])->attachGoal(theGoals[30]);

	((theCriteria)[20])->attachGoal(theGoals[22]);
	((theCriteria)[20])->attachGoal(theGoals[27]);
	((theCriteria)[20])->attachGoal(theGoals[31]);

	((theCriteria)[21])->attachGoal(theGoals[23]);
	((theCriteria)[21])->attachGoal(theGoals[27]);
	((theCriteria)[21])->attachGoal(theGoals[32]);

	((theCriteria)[22])->attachGoal(theGoals[24]);
	((theCriteria)[22])->attachGoal(theGoals[28]);
	((theCriteria)[22])->attachGoal(theGoals[33]);

	((theCriteria)[23])->attachGoal(theGoals[25]);
	((theCriteria)[23])->attachGoal(theGoals[28]);
	((theCriteria)[23])->attachGoal(theGoals[34]);

	((theCriteria)[24])->attachGoal(theGoals[29]);
	((theCriteria)[24])->attachGoal(theGoals[37]);

	((theCriteria)[25])->attachGoal(theGoals[30]);
	((theCriteria)[25])->attachGoal(theGoals[35]);
	((theCriteria)[25])->attachGoal(theGoals[38]);

	((theCriteria)[26])->attachGoal(theGoals[31]);
	((theCriteria)[26])->attachGoal(theGoals[35]);
	((theCriteria)[26])->attachGoal(theGoals[39]);

	((theCriteria)[27])->attachGoal(theGoals[32]);
	((theCriteria)[27])->attachGoal(theGoals[36]);
	((theCriteria)[27])->attachGoal(theGoals[40]);

	((theCriteria)[28])->attachGoal(theGoals[33]);
	((theCriteria)[28])->attachGoal(theGoals[36]);
	((theCriteria)[28])->attachGoal(theGoals[41]);

	((theCriteria)[29])->attachGoal(theGoals[34]);
	((theCriteria)[29])->attachGoal(theGoals[42]);

	((theCriteria)[30])->attachGoal(theGoals[37]);
	((theCriteria)[30])->attachGoal(theGoals[43]);
	((theCriteria)[30])->attachGoal(theGoals[46]);

	((theCriteria)[31])->attachGoal(theGoals[38]);
	((theCriteria)[31])->attachGoal(theGoals[43]);
	((theCriteria)[31])->attachGoal(theGoals[47]);

	((theCriteria)[32])->attachGoal(theGoals[39]);
	((theCriteria)[32])->attachGoal(theGoals[44]);
	((theCriteria)[32])->attachGoal(theGoals[48]);

	((theCriteria)[33])->attachGoal(theGoals[40]);
	((theCriteria)[33])->attachGoal(theGoals[44]);
	((theCriteria)[33])->attachGoal(theGoals[49]);

	((theCriteria)[34])->attachGoal(theGoals[41]);
	((theCriteria)[34])->attachGoal(theGoals[45]);
	((theCriteria)[34])->attachGoal(theGoals[50]);

	((theCriteria)[35])->attachGoal(theGoals[42]);
	((theCriteria)[35])->attachGoal(theGoals[45]);
	((theCriteria)[35])->attachGoal(theGoals[51]);
	
	((theCriteria)[36])->attachGoal(theGoals[46]);
	((theCriteria)[36])->attachGoal(theGoals[54]);

	((theCriteria)[37])->attachGoal(theGoals[47]);
	((theCriteria)[37])->attachGoal(theGoals[52]);
	((theCriteria)[37])->attachGoal(theGoals[55]);

	((theCriteria)[38])->attachGoal(theGoals[48]);
	((theCriteria)[38])->attachGoal(theGoals[52]);
	((theCriteria)[38])->attachGoal(theGoals[56]);

	((theCriteria)[39])->attachGoal(theGoals[49]);
	((theCriteria)[39])->attachGoal(theGoals[53]);
	((theCriteria)[39])->attachGoal(theGoals[57]);

	((theCriteria)[40])->attachGoal(theGoals[50]);
	((theCriteria)[40])->attachGoal(theGoals[53]);
	((theCriteria)[40])->attachGoal(theGoals[58]);

	((theCriteria)[41])->attachGoal(theGoals[51]);
	((theCriteria)[41])->attachGoal(theGoals[59]);

	((theCriteria)[42])->attachGoal(theGoals[54]);
	((theCriteria)[42])->attachGoal(theGoals[60]);

	((theCriteria)[43])->attachGoal(theGoals[55]);
	((theCriteria)[43])->attachGoal(theGoals[60]);
	((theCriteria)[43])->attachGoal(theGoals[63]);
		
	((theCriteria)[44])->attachGoal(theGoals[56]);
	((theCriteria)[44])->attachGoal(theGoals[61]);
	((theCriteria)[44])->attachGoal(theGoals[64]);

	((theCriteria)[45])->attachGoal(theGoals[57]);
	((theCriteria)[45])->attachGoal(theGoals[61]);
	((theCriteria)[45])->attachGoal(theGoals[65]);

	((theCriteria)[46])->attachGoal(theGoals[58]);
	((theCriteria)[46])->attachGoal(theGoals[62]);
	((theCriteria)[46])->attachGoal(theGoals[66]);

	((theCriteria)[47])->attachGoal(theGoals[59]);
	((theCriteria)[47])->attachGoal(theGoals[62]);

	((theCriteria)[48])->attachGoal(theGoals[63]);
	((theCriteria)[48])->attachGoal(theGoals[67]);

	((theCriteria)[49])->attachGoal(theGoals[64]);
	((theCriteria)[49])->attachGoal(theGoals[67]);
	((theCriteria)[49])->attachGoal(theGoals[69]);

	((theCriteria)[50])->attachGoal(theGoals[65]);
	((theCriteria)[50])->attachGoal(theGoals[68]);
	((theCriteria)[50])->attachGoal(theGoals[70]);

	((theCriteria)[51])->attachGoal(theGoals[66]);
	((theCriteria)[51])->attachGoal(theGoals[68]);

	((theCriteria)[52])->attachGoal(theGoals[69]);
	((theCriteria)[52])->attachGoal(theGoals[71]);
	
	((theCriteria)[53])->attachGoal(theGoals[70]);
	((theCriteria)[53])->attachGoal(theGoals[71]);

}

//passes in the seed and using random generator to create our board data.
void Board::initSeed(int seed){
    srand(seed);
    theTiles.resize(0);    
    theGoals.resize(0);
    theCriteria.resize(0);
    bool noLongerNetflix = false;

    // construct tiles using random generator
    for(int i = 0; i < 19; ++i) {
	string stype;
	int type = rand() % 6;
	int val = (rand() % 11) + 2;
	if ((type == 5) && (noLongerNetflix == true)) {
	    while(type == 5) {
		type = rand() % 6;
	    }
        }
	if (type == 0) {
	    stype = "CAFFEINE";
	} else if (type == 1) {
	    stype = "LAB";
	} else if (type == 2) {
	    stype = "LECTURE";
	} else if (type == 3) {
	    stype = "STUDY";
	} else if (type == 4) {
	    stype = "TUTORIAL";
	} else if (type == 5) {
	    stype = "NETFLIX";
	    noLongerNetflix = true;
	}
	if (stype == "NETFLIX") {
	    val = 7;
        } else {
	    while(val == 7) {
		val = (rand() % 11) + 2;
	    }
	}
	Tile *tile = new Tile{i, stype, val};
	theTiles.push_back(tile);
	if (stype == "NETFLIX") {
	    Tile *curGeese = theTiles[i];
    	    curGeese->setGeese(true);
    	    lastGeese = curGeese;	    
	}
    }

    // construct goals
    for(int i = 0; i < 72; ++i) {
	Goal *goal = new Goal{i};
	theGoals.push_back(goal);
    }

    // construct criterion
    for(int i = 0; i < 54; ++i) {
	Criteria *criteria = new Criteria{i};
	theCriteria.push_back(criteria);
    }

    //assign criterion/goals
    for (int i = 0; i < 19; i++) {
        Position pos = Util::getTilePosition(i);

        int* coursesAssigned = Util::getTileVertexDistribution(i);
        for (int idx = 0; idx < 6; idx++) {
            theTiles[i]->attach(theCriteria[coursesAssigned[idx]]);
        }

        int* goalsAssigned = Util::getTileEdgeDistribution(i);
        for (int idx = 0; idx < 6; idx ++) {
            theTiles[i]->setGoal(theGoals[goalsAssigned[idx]], idx);
        }
    }

    td = new TextDisplay(theTiles);
    this->attachAllNeighbours();
}
