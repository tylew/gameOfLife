#include <iostream>
#include <unistd.h>
#include "cellMap.h"
using namespace std;

int main(int argc,char** argv){
  //Initiate necessary variables and instance of a cellMap
  string choice;
  string fileName = "";
  cellMap* simulation = new cellMap();

  //Capture user choice and pass on appropriate information to cellMap class
  cout << "\nWould you like to provide a map file to read from? \n- You will either be prompted to provide a file name, or for the desired dimensions/density of a randomly generated map. \n 1. Load from file \n 2. Random generation" << endl;

  while(choice!="1"&&choice!="2"){
    cout << "--> ";
    getline(cin,choice);
    cout << endl;
  }if(choice=="1"){

    cout << "Please specify the file name which you would like to read from" << endl;

    while(fileName == ""){
      cout << "--> ";
      getline(cin,fileName);
      cout << endl;
    }
    simulation->loadFile(fileName); //Load from file

  }else{
    int rc;
    int cc;
    double ds;

    cout << "Please specify the desired amount of rows\n--> ";
    cin >> rc;
    cin.ignore();

    cout << "\nPlease specify the desired amount of collumns\n--> ";
    cin >> cc;
    cin.ignore();

    cout << "\nPlease specify the desired starting density of living cells (decimal between 0 and 1)\n--> ";
    cin >> ds;
    cin.ignore();

    simulation->loadRandom(rc,cc,ds); //Load random simulation with user input characteristics
  }

  cout << "\nPlease choose a mode for edge handling.\n 1. Classic (off the edge = no cells)\n 2. \"Doughnut\" (consider opposite edge, map wraps around itself)\n 3. Mirror (Edge items are mirrored)\n--> ";
  int modeChoice;
  cin >> modeChoice;
  cin.ignore();
  cout << endl;
  simulation->setMode(modeChoice);

  cout << "What to do between generations?\n 1. Press enter key\n 2. Brief pause\n 3. Output to file\n--> ";

  cin >> modeChoice;
  cin.ignore();
  cout << endl;

  if(modeChoice==3){
    cout << "What is the name of the file you would like to print to?\n--> ";
    cin >> fileName;
    cout << endl;
  }

  while(simulation->stillGoing()){ //Check if grid empty or stabalized. If not, run another generation
    simulation->nextGen();


    if(modeChoice==1){
      cout << "Press Enter to Continue\n";
      cin.ignore();
    }if(modeChoice==2){
      usleep(250000);
    }if(modeChoice==3){
      simulation->printMapToFile(fileName);
    }
  }

  if(modeChoice==3){ //confirmation that file has been written to
    for(int i=0;i<60;++i){ //clear screen
      cout << endl;
    }
    cout << "Program finished successfully and wrote to file " << fileName << endl << "\nPress enter to exit..." << endl;
    cin.ignore();
  }

  cin.ignore();

  simulation->~cellMap(); //deconstruct
  return 0;
}
