#include <iostream>
#include "board.h"
#include <string>
#include <sstream>
#include "student.h"
#include <vector>
#include "tile.h"
#include "criteria.h"
#include "goal.h"
#include "textdisplay.h"
#include "position.h"
#include "util.h"
#include <fstream>
#include "load.h"

using namespace std;


void save(int turn, Student **studArr, Board &b, string filename) {
    ofstream ofs("save.txt");
    ofs << turn << endl ;
    for(int i = 0; i < 4; i++) {
        ofs << studArr[i]->printSave() << endl ;
    }
    ofs << b.printSave() << endl ;
    ofs << b.getGeesePos() << endl ;
	for(int j = 0; j < 4; ++j) {
               delete studArr[j];
         }
       Util::release();
}

int main(int argc, char *argv[]) {
    Board b;
    bool isBeginning = false;
    int turn = 0 ;
    bool endGame = false;

    //Initializing Students
    Student *blue = new Student{"BLUE"};
    Student *yellow = new Student{"YELLOW"};
    Student *orange = new Student{"ORANGE"};
    Student *red = new Student{"RED"};
    Student *studArr[] = {blue, red, orange, yellow} ;
     
    
    //1. Command Line Dealings
    //If we are not given 2 command line inputs (e.g. -load xxx) then cant do stuff
    if(argc != 3) {
        cerr << "ERROR: INVALID CMD LINE ARGS" << endl;
        return 1;
    }
    //These variables keep track of how we will initialize the game and the info needed to do so
    // e.g. initType = Load, initSource = "myfile.txt"
    string initType = argv[1] ;
    string initSource = argv[2] ;
    
    // Call the appropriate board initialization function depending on cmd line args
    // Functions/Methods Used: initSeed, initBoard, Load
    if(initType == "-seed") { 
	istringstream buffer(initSource);
	int numSeed;
	buffer >> numSeed;	
        b.initSeed(numSeed);
	b.displayBoard();
        isBeginning = true;
    } else if(initType == "-load") {
        Load::load(initSource, studArr, b, turn);
        b.displayBoard();
	isBeginning = false;
    } else if(initType == "-board" ){
        Load::board(initSource, b);
	b.displayBoard();
        isBeginning = true;
    } else {
        cerr << "ERROR: INVALID BOARD INITIALIZATION TYPE" << endl;
	return 1;
    }
    
      
    // 2. Initial Assignment placements for each student
    if(isBeginning){
    for(int i = 0; i < 4; i++) {
        Student *currStud = studArr[i];
        
        cout << "Student " << currStud->getStudentColour() << " where do you want to complete an Assignment?" << endl;
	cout << "> ";
        int pos;
	cin >> pos;
	if (cin.fail()) {
	    save(turn, studArr, b, "save.txt");
	    return 0;
	}
        
        try{
        b.buildCriteria(currStud, pos, true) ;
        } catch (string &s) { 
            cout << "Error: " << s << endl << "Please retry placement." << endl ; 
            i = i - 1 ; 
        }
        
    } 
    // Still doing Initial Assignments, just reverse order
    for(int i = 3; i >= 0; i--) {
        Student *currStud = studArr[i] ;
    
        cout << "Student " << currStud->getStudentColour() << " where do you want to complete an Assignment?" << endl ;
        cout << "> ";
	int pos;
	cin >> pos;
        if (cin.fail()) {
	    save(turn, studArr, b, "save.txt");
	    return 0;
	}

        try{
            b.buildCriteria(currStud, pos, true) ;
        } catch (string &s) { 
            cout << "Error: " << s << endl << "Please retry placement." << endl ; 
            i = i + 1 ; 
        }
	    
    } 
    b.displayBoard();  
    }
        
    
    // 3. Start the regular gameplay
    for(int i = turn % 4; i <= 4; i++) {
	if (endGame == true) {
	    save(turn, studArr, b, "save.txt");
	    break;
	}
        if(i == 4) i = 0 ; 
        Student *currStud = studArr[i];
        
        //Print whose turn it is
	cout << "Turn " << turn << endl;
        cout << "Student " << currStud->getStudentColour() << "'s turn." << endl ;
        currStud->printStatus();
        
        
        // 3a) Dice selection, rolling, and resource collection
        string diceCmd;
        int diceRollVal = 0; //hard code a dice roll.
        while(true){
	    cout << ">";
	    cin >> diceCmd; 
	    if (cin.fail()) {
		save(turn, studArr, b, "save.txt");
		return 0;
	    }

            if(diceCmd == "roll") {
                try {
		    diceRollVal = currStud->studentRoll();
                    break;
		} catch(string &s) {
		    cout << "Error: " << s << "\n";
		}   
            } else if(diceCmd == "load") {
                currStud->setTypeOfDice("load");
            } else if(diceCmd == "fair") {
                currStud->setTypeOfDice("fair");
            } else {
                cout << "Invalid Command! Valid Commands: [roll, load, fair]" << endl;
            }
        }
        // **STUFF** based on number of roll, the tiles will notify the criterion or GEESE
      	cout << "Roll is: " << diceRollVal << endl ; 
	if (diceRollVal != 7){
            b.yieldResources(diceRollVal);
	    int noResource = 0;
            for (int i = 0; i <4; i++){
                noResource += studArr[i]->displayResourceIncome();
            }
            if (noResource == 0){
                cout << "No student gained resources" << endl;
            }   
        } else {
            //everyone with 10 or more resources lose things!
            for (int i = 0; i < 4; i++){
                studArr[i]->loseResources();
            }
            //now we set geese and steal things. 
            cout << "Choose where to place the GEESE" << endl;
            int pos;
            cin >> pos;
            b.setGeese(pos, currStud);
        }
        
        //3b) End of Turn commands 
        cout << "End of Turn commands" << endl;
	cout << "[next]";
	cout << "[board]";
	cout << "[status]";
	cout << "[criteria]";
	cout << "[achieve]" << endl;
	cout << "[complete]" ;
	cout << "[improve]" ;
	cout << "[trade]" ;
	cout << "[save]" << endl;

        while(true) {
	    cout << "> ";
            string command;
            cin >> command;
            if (command == "next") {
                turn++;
                break ;
                
                
            }
            else if (command == "board") {
                b.displayBoard();
                
                
            } else if (command == "status") {
                //"STUDENT1: " etc.
                for(int j = 0; j < 4; j++) {
                    studArr[j]->printStatus();
                }
                
                
            } else if (command == "criteria") {
                currStud->printCriteria(); 
		                
            } else if (command == "achieve") {
                try{
                    int pos ;
                    cin >> pos ;
                    b.buildGoal(currStud, pos, false) ;
                } catch(string &s) { 
                    cout << "Error: " << s << endl << "Try \"help\" for assistance" << endl ; 
                }
                
                
            } else if (command == "complete") {
                try{
                    int pos ;
                    cin >> pos ;
                    b.buildCriteria(currStud, pos, false) ;
                } catch(string &s) {
                    cout << "Error: " << s << endl << "Try \"help\" for assistance" << endl ; 
                }
                if (currStud->isWon()){
                    cout << currStud->getStudentColour() << " wins!";
                    break;
                }
                
                
            } else if (command == "improve") {
                try{
                    int pos ;
                    cin >> pos ;
                    b.improveCriteria(currStud, pos, false) ;
                } catch(string &s) {
                    cout << "Error: " << s << endl << "Try \"help\" for assistance" << endl ; 
                }
                if (currStud->isWon()){
                    cout << currStud->getStudentColour() << " wins!";
                    break;
                }
                
                
            } else if (command == "trade") { 
       		string otherColour;
		string give;
                string take;
		string yesOrNo;

                cout << "Who do you want to trade with?\n> ";
                cin >> otherColour;
		cout << "What resource would you like to give?\n> ";
                cin >> give;
		cout << "What resource would you like to take?\n> ";
                cin >> take;
       		cout << otherColour << ", do you accept this trade? (yes/no)\n> ";
		cin >> yesOrNo;
         
		if (yesOrNo == "yes") { 
                    Student *otherPlayer = nullptr;
                    for (int i = 0; i < 4; i++){
                        if (otherColour == studArr[i]->getStudentColour()){
                            otherPlayer = studArr[i];
                        }
                    }
                    try {
                        currStud->tradeResources(otherPlayer, give, take);
                    } catch(string &s) {
                        cout << "Error: " << s << endl << "Try \"help\" for assistance" << endl ; 
                    }
		}
                
            } else if (command == "save") { 
		string file ; 
		cin >> file ; 

		ofstream ofs(file);
    		ofs << turn << endl ;
    		for(int i = 0; i < 4; i++) {
        		ofs << studArr[i]->printSave() << endl ;
    		}
   		ofs << b.printSave() << endl ;
    		ofs << b.getGeesePos() << endl ;
		cout << "here" << endl;
            } else if (command == "help") {
                cout << "Valid commands:\nboard\nstatus\ncriteria" << endl;
                cout << "achieve <goal>\ncomplete <criterion>\nimprove <criterion>" << endl;
                cout << "trade <colour> <give> <take>\nnext\nsave <file>\nhelp" << endl;
        
                
            } else {
		if (cin.fail()) {
		    endGame = true;
		    cin.clear();
		    save(turn, studArr, b, "save.txt");
                    return 0;
		}

                cout << "Invalid Command, try asking for help!" << endl ;
		cin.clear();
            }
        }
        
        if (currStud->isWon()){
            cout << "Would you like to play again?" << endl;
	    cout << "> ";
	    string yesOrNo;
	    cin >> yesOrNo;
	    if (yesOrNo == "yes") {
		for(int j = 0; j < 4; ++j) {
		    (studArr[j])->resetStudent();
		}
	        string snumSeed = "\0";
        	cin >> snumSeed;
        	istringstream buffer(snumSeed);
        	int numSeed;
        	buffer >> numSeed;
		b.initSeed(numSeed);
	    } else if (yesOrNo == "no") {
            	break;
	    }
        }
     
       
    } //#3 for-loop

    for(int j = 0; j < 4; ++j) {
	delete studArr[j];
    } 
    Util::release();    
    
} // main.


