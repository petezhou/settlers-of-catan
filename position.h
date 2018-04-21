#ifndef position_hpp
#define position_hpp

class Position{
    int row;
    int col;
public:
   // helpful for Textdisplay
    Position();
    Position(int row, int col);
    ~Position();
    void setRow(int row);
    void setCol(int col);
    int getRow();
    int getCol();
};


#endif 

