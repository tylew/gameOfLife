#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;
//load file, load random, set mode, still going, next gen, printMapToFile
class cellMap{
  public:
    cellMap(); //constructor
    ~cellMap();//Deconstructor
    void loadRandom(int rows, int colm, double popDense); //loads random grid with given params
    void loadFile(string fn); //Load grid from file
    bool stillGoing(); //checks if generations still changing, returns status boolean
    void setMode(int mode); //sets game mode, (classic, doughnut, mirror)
    void nextGen(); //performs necessary tasks to convert genHistory scors to mapArr
    void printMapToFile(string fn); //Print current mapArr to specified file
private:
    void printMap(); //print current mapArr **
    void printScoreMap(); //Print current genHistory **
    int getNum(int max); //gets random number between 0 and max.
    void getNeighbors(); //Get neighbor score for all cells except first/last column and row of current mapArr
    void getClassicNeighbors(); //Calc neighbor score for classic game mode for first/last column and row of current mapArr
    void getDoughnutNeighbors(); //Calc neighbor score for doughnut game mode for first/last column and row of current mapArr
    void getMirrorNeighbors(); //Calc neighbor score for mirror game mode for first/last column and row of current mapArr
    void clearScr(); //Print 60 or so lines of blank to clear screen

    int gameMode; //1 is classic, 2 is donut, 3 is mirror, 0 is null
    int genCount; //keeps track of current generation
    bool status; //True if game still going, false once map empty or stabalized
    char** mapArr; //Holds current map
    int** genHistory; //Holds current score of each item on mapArr
    int rowCount; //Amount of rows for current simulation
    int colCount; //Amount of collumns for current simulation
    int totalCount; //Total amount of spaces in grid for current simulation
    int cellCount; //Total starting living cells in current simulation
};
