#ifndef LOAD_H
#define LOAD_H
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
#include "fstream"
using namespace std;
class Load {
public:
    static void load(string filename, Student *studArr[], Board &b, int &turn) { 
	ifstream inFile{filename} ;
		string templine ;
		vector<string> theLines ; 
		for(int i =0; i < 7; i++) {
			getline(inFile, templine) ; 
			theLines.push_back(templine) ; 
		}
		// 0 is turn 
		// 1-4 are students 
		// 5 is board 
		// 6 is setGeese 
		// First we need to initialize the board 
		string line5 = theLines.at(5) ; 
		vector<int> board;
	    string curNum = "";
	    int actualNum;
	    for(int j = 0; j < line5.length(); j++){
	        if (line5[j] == ' '){
	            istringstream iss {curNum};
	            iss >> actualNum;
	            board.push_back(actualNum);
	            curNum = "";
	        } else {
	            curNum += line5[j];
	        }
	    }
	    if (curNum != "") {
		istringstream iss {curNum};
                iss >> actualNum;
                board.push_back(actualNum);
             }
	    //Board Init using the vector
	    b.initBoard(board) ; 
	    //Now we can do the rest
	    // Setting the turn 
	    string line0 = theLines.at(0) ; 
	    string curNu = "";
		int theNum;
		for(int j = 0; j <= line0.length(); j++){
	        if (line0[j] == ' ' || j == line0.length()){
	            istringstream iss {curNu};
	            iss >> theNum;
	            curNu = "";
	        } else {
	            curNu += line0[j];
	        }
	    }
		turn = theNum;
		// Initializing the Students 
		for(int i = 1; i < 5; i++) {
			string line = theLines.at(i);
			vector<int> resources ;  
			vector<int> goals ; 
			vector<int> criteria ; 
			int curIndex = 0;
			//First build student with inputted resource levels 
			string curNum = "";
		    int actualNum;
		    for(int j = 0; line[j] != 'g'; j++){
		        if (line[j] == ' ' ){
		            istringstream iss {curNum};
		            iss >> actualNum;
		            resources.push_back(actualNum);
		            curNum = "";
		        } else {
		            curNum += line[j];
		        }
			curIndex++ ; 
		    }
			//Set Resources
			studArr[i-1]->setResources(resources) ; 
			//Second we build all the goals associated with that student 
			//Fill Goals Array with numbers indicating location
			curNum = "";
		        actualNum = 0;
			curIndex += 2 ;
			for(int j = curIndex; line[j] != 'c'; j++){
				curIndex++ ; 
			 if (line[j] == ' '){
		            istringstream iss {curNum};
		            iss >> actualNum;
		            goals.push_back(actualNum);
		            curNum = "";
		        } else {
		            curNum += line[j];
		        }
			}
			//Build the goals onto board with appropriate location and student 
			for(int j = 0; j < goals.size(); j++) {
				b.buildGoal(studArr[i-1], goals.at(j), true) ; 
			}
			//Lastly we build all the criteria associated with the student
			//Filling the array with positions of the criteria
			curNum = "";
			for(int j = curIndex + 2; j < line.length(); j++){
				if (line[j] == ' '){
		        	    istringstream iss {curNum};
		           	    iss >> actualNum;
		          	    criteria.push_back(actualNum);
		           	    curNum = "";
		        	} else {
		            		curNum += line[j];
		        	}
			}
			if (curNum != ""){
			    istringstream iss {curNum};
			    iss >> actualNum;
			    criteria.push_back(actualNum);
		        } 
			//Build the Criteria onto board with appropriate location and student 
			for(int j = 0; j < criteria.size(); j += 2) {
				if(criteria.at(j + 1) == 1) {
					b.buildCriteria(studArr[i-1], criteria.at(j), true) ; 
				} else if(criteria.at(j + 1) == 2) {
					b.buildCriteria(studArr[i-1], criteria.at(j ), true) ; 
					b.improveCriteria(studArr[i-1], criteria.at(j ), true) ;
				} else if(criteria.at(j + 1) == 3) {
					b.buildCriteria(studArr[i-1], criteria.at(j), true) ; 
					b.improveCriteria(studArr[i-1], criteria.at(j ), true) ;
					b.improveCriteria(studArr[i-1], criteria.at(j ), true) ;
				}
			}
		}
		// Now we set the Geese
		string line6 = theLines.at(6) ; 
	        string s = line6;
		int n;
                istringstream iss {s};
		iss >> n;
		b.lastGeese->setGeese(false);
	        b.getTiles().at(n)->setGeese(true) ;
		b.lastGeese = b.getTiles().at(n);
    }

    static void seed(int seed, Board &b) {
        //Some shit
    }
    
    static void board(string filename, Board &b) {
        ifstream inFile {filename} ;
        string line ;
        getline(inFile, line) ;
    
        //Fill vector with the numbers in the line
        vector<int> board;
        string curNum = "";
        int actualNum;
        for(int j = 0; j < line.length(); j++){
            if (line[j] == ' '){
                istringstream iss {curNum};
                iss >> actualNum;
                board.push_back(actualNum);
                curNum = "";
            } else {
                curNum += line[j];
            }
        }
        //Board Init using the vector
        b.initBoard(board) ;
    }
};
#endif
