#include "cellMap.h"

using namespace std;

//Constructor
cellMap::cellMap(){
  clearScr();
}

//Deconstructor
cellMap::~cellMap(){}

//Create a map with specified size and density
void cellMap::loadRandom(int rows, int colm, double popDense){

  //Prevent duplicate random
  srand(time(0));

  //initiate ARRAYS and other variables
  gameMode = 0;
  genCount = 1;
  rowCount = rows;
  colCount = colm;
  totalCount = rowCount * colCount;
  cellCount = totalCount * popDense;

  //*[0]=current generation, *[1]= parent, *[2]= grandparent
  genHistory = new int*[totalCount];
  for(int i = 0;i<totalCount;++i){
    genHistory[i] = new int[3];
  }

  //allocate memory for map
  mapArr = new char*[rowCount];
  for(int i = 0;i<rowCount;++i){
    mapArr[i] = new char[colCount];
  }

  //create map with no living cells
  for(int i = 0;i<rowCount;++i){
    for(int j = 0;j<colCount;++j){
      mapArr[i][j]='-';
    }
  }

  //fill map with specified density of living cells
  int r,c;
  int count = 0;
  if(cellCount<=totalCount){
    while(count<cellCount){
      r = getNum(rowCount);
      c = getNum(colCount);

      if(mapArr[r][c]=='X'){ //make sure all of living cell count is accounted for
        continue;
      }
      mapArr[r][c] = 'X';
      ++count;
    }
    status = true;
  }else{
    status=false;
    genCount=0;
  }
}

//Load file to mapArr from specified file path
void cellMap::loadFile(string fn){
  //Open input stream and initiate vars
  ifstream inFS;
  string placeholder;
  inFS.open(fn);

  if (inFS.is_open()){ //make sure opened properly
    getline(inFS,placeholder);
    rowCount = std::stoi(placeholder);
    getline(inFS,placeholder);
    colCount = std::stoi(placeholder);

    //initiate ARRAYS and other variables
    gameMode = 0;
    genCount = 1;
    totalCount = rowCount * colCount;

    //[0]=current generation, [1]= parent, [2]= grandparent
    genHistory = new int*[totalCount];
    for(int i = 0;i<totalCount;++i){
      genHistory[i] = new int[3];
    }

    mapArr = new char*[rowCount];
    for(int i = 0;i<rowCount;++i){
      mapArr[i] = new char[colCount];
    }

    for(int i = 0;i<rowCount;++i){
      getline(inFS,placeholder);
      for(int j = 0;j<colCount;++j){
        mapArr[i][j]=placeholder[j];
      }
    }
    status = true;
  }else{ //if didnt open properly, this will end simulation
    cout << "Error: could not open read file: " << fn << endl;
    status = false;
  }
  inFS.close(); //close input stream
}

//used in main to determine whether to run another generation or not.
bool cellMap::stillGoing(){
  if(!status){
    if(genCount>1){
      cout << "Ran " << genCount-1 << " itterations before fully stabalized. Press enter to exit.\n" << endl; //This happens a lot...
    }else{
      cout << "Was unable to run program.\n" << endl; //Only if something REALLY went wrong...
    }
  }
  if(genCount==150+totalCount/5){
    nextGen(); //Make sure count adds up properyl
    status=false;
    cout << "Simulation lasts for infinity. Stopped at "<< 150+totalCount/5 << " generations." << endl;
  }
  return status;
}

//used in main to set game mode from user input
void cellMap::setMode(int mode){
  if(mode == 1||mode==2||mode==3){
    status = true;
  }else{
    cout << "Game mode not specified." << endl;
    status = false; //makes sure game wont run if gibberish is input by user.
  }
  gameMode = mode;
}

//Using genHistory array, update mapArr to next generation
void cellMap::nextGen(){
  //Update genHistory according to current map.
  getNeighbors();

  int neighborAvr;
  int dv;
  //makes sure for the first 2 generations that the average is calculated correctly, and by 3 for future generations.
  if(genCount<3){
    dv = genCount;
  }else{
    dv = 3;
  }
  printMap();

  //change map according to generation history
  for(int r = 0;r<rowCount;++r){
    for(int c = 0;c<colCount;++c){
      neighborAvr = (genHistory[r*colCount+c][0]+genHistory[r*colCount+c][1]+genHistory[r*colCount+c][2])/dv;
      if(neighborAvr < 1.5){
        mapArr[r][c]='-';
        continue;
      }if(neighborAvr>=1.5&&neighborAvr<=2.5){
        continue;
      }if(neighborAvr>2.5&&neighborAvr<=3.5){
        mapArr[r][c]='X';
        continue;
      }if(neighborAvr>3.5){
        mapArr[r][c]='-';
        continue;
      }
    }
  }
  dv=0; //use already declared variable dv to track whether game is still in play...
  for(int r = 0;r<totalCount;++r){
    if(genHistory[r][0]!=genHistory[r][1]||genHistory[r][1]!=genHistory[r][2]){
      ++dv;
    }
  }
  if(dv==0){
    status = false;//If the whole map has the same value for past 3 generations then game will end.
  }

  ++genCount; //keep track of generation #
}

//clear screen
void cellMap::clearScr(){
  //print 70 blank lines in case someone is in full screen,hopefully monitor not big enough for >70 lines. if so, that sucks for them 😂
  for(int i = 0;i<70;++i){
    cout << endl;
  }
}

//Print current map (visualize in terminal)
void cellMap::printMap(){
  clearScr();
  cout << "Generation #" << genCount << endl;
  for(int i = 0;i<rowCount;++i){ //make sure each new row is on a new line
    for(int j = 0;j<colCount;++j){
      cout << mapArr[i][j];
    }
    cout << endl;
  }
}

//Print score for current map in same format as printMap (but with numbers indicatins # neighbors) without generation counter
void cellMap::printScoreMap(){
  for(int i = 0;i<rowCount;++i){
    for(int j = 0;j<colCount;++j){
      cout << genHistory[i*colCount+j][0]; //do you read all my comments?
    }
    cout << endl;
  }
}

//Print map to file in same format as printMap
void cellMap::printMapToFile(string fn){
  ofstream outFS; //open output stream

  outFS.open(fn, fstream::app);//makes sure appending to file rather than overwriting

  if(!outFS.is_open()){
    cout << "Error: could not open write file: " << fn << endl; //oopsie :(
    status = false; //ends game in main
    return;
  }else{
    outFS << "Generation #" << genCount << endl;
    for(int i = 0;i<rowCount;++i){ //itterate thru whole array, format correctly
      for(int j = 0;j<colCount;++j){
        outFS << mapArr[i][j];
      }
      outFS << endl;
    }
  }
}

//attain random int between 0 and max
int cellMap::getNum(int max){
  return rand() % (max);;
}

/*mode 1 is classic, mode 2 is doughnut, mode 3 is mirror
All this method does is update int** genHistory*/
void cellMap::getNeighbors(){
  int count;

  //Everything except first/last row/collumn
  for(int i = 1;i<rowCount-1;++i){
    for(int j = 1;j<colCount-1;++j){
      count = 0;

      for(int z = -1;z<=1;++z){
        for(int g = -1;g<=1;++g){
          if(z==0&&g==0){
            continue;
          }
          if(mapArr[i+z][j+g]=='X'){
            ++count;
      }}}

      genHistory[i*colCount+j][2]=genHistory[i*colCount+j][1];
      genHistory[i*colCount+j][1]=genHistory[i*colCount+j][0];
      genHistory[i*colCount+j][0]=count;
    }}

  //Call proper metod depending on game mode
  if(gameMode == 1){
    getClassicNeighbors();
  }else if(gameMode == 2){
    getDoughnutNeighbors();
  }else if(gameMode == 3){
    getMirrorNeighbors();
  }

}

//Determine edge scores for Classic mode
void cellMap::getClassicNeighbors(){
  //Top left corner
  int count = 0;
  for(int i = 0;i<=1;++i){
      if(mapArr[1][i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[0][1]=='X'){
        ++count;
      }
  }
  genHistory[0][2]=genHistory[0][1];
  genHistory[0][1]=genHistory[0][0];
  genHistory[0][0]=count;
  //Top right corner
  count = 0;
  for(int i = -1;i<=0;++i){
      if(mapArr[1][colCount-1+i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[0][colCount-2]=='X'){
        ++count;
      }
  }
  genHistory[colCount-1][2]=genHistory[colCount-1][1];
  genHistory[colCount-1][1]=genHistory[colCount-1][0];
  genHistory[colCount-1][0]=count;
  //Bottom left corner
  count = 0;
  for(int i = 0;i<=1;++i){
      if(mapArr[rowCount-2][i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[rowCount-1][1]=='X'){
        ++count;
      }
  }
  genHistory[totalCount-colCount][2]=genHistory[totalCount-colCount][1];
  genHistory[totalCount-colCount][1]=genHistory[totalCount-colCount][0];
  genHistory[totalCount-colCount][0]=count;
  //Bottom right corner
  count = 0;
  for(int i = -1;i<=0;++i){
      if(mapArr[rowCount-2][colCount-1+i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[rowCount-1][colCount-2]=='X'){
        ++count;
      }
  }
  genHistory[totalCount-1][2]=genHistory[totalCount-1][1];
  genHistory[totalCount-1][1]=genHistory[totalCount-1][0];
  genHistory[totalCount-1][0]=count;
  //First collumn except corners
  for(int h = 1;h<rowCount-1;++h){
    count = 0;
    for(int i=0;i<=1;++i){
      if(mapArr[h-1][i]=='X'){
        ++count;
      }
      if(mapArr[h+1][i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[h][1]=='X'){
        ++count;
      }
    }
    genHistory[colCount * h][2]=genHistory[colCount * h][1];
    genHistory[colCount * h][1]=genHistory[colCount * h][0];
    genHistory[colCount * h][0]=count;

  }
  //Last collumn except corners
  for(int h = 1;h<rowCount-1;++h){
    count = 0;
    for(int i=-1;i<=0;++i){
      if(mapArr[h-1][colCount-1+i]=='X'){
        ++count;
      }
      if(mapArr[h+1][colCount-1+i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[h][colCount-2]=='X'){
        ++count;
      }
    }
    genHistory[colCount * (h+1)-1][2]=genHistory[colCount * (h+1)-1][1];
    genHistory[colCount * (h+1)-1][1]=genHistory[colCount * (h+1)-1][0];
    genHistory[colCount * (h+1)-1][0]=count;

  }
  //First row except corners
  for(int i = 1;i<colCount-1;++i){
    count = 0;
    for(int j = -1; j <= 1; ++j){
      if(mapArr[1][i+j]=='X'){
        ++count;
      }
      if(j==0){
        continue;
      }
      if(mapArr[0][i+j]=='X'){
        ++count;
      }
    }
    genHistory[i][2]=genHistory[i][1];
    genHistory[i][1]=genHistory[i][0];
    genHistory[i][0]=count;
  }
  //Last row except corners
  for(int i = 1;i<colCount-1;++i){
    count = 0;
    for(int j = -1; j <= 1; ++j){
      if(mapArr[rowCount-2][i+j]=='X'){
        ++count;
      }
      if(j==0){
        continue;
      }
      if(mapArr[rowCount-1][i+j]=='X'){
        ++count;
      }
    }
    genHistory[((rowCount-1)*colCount)+i][2]=genHistory[(rowCount-1)*colCount+i][1];
    genHistory[(rowCount-1)*colCount+i][1]=genHistory[(rowCount-1)*colCount+i][0];
    genHistory[(rowCount-1)*colCount+i][0]=count;
  }
}

//Determine edge scores for Doughnut mode
void cellMap::getDoughnutNeighbors(){
  //Top left corner
  int count = 0;
  for(int i = 0;i<=1;++i){
      if(mapArr[1][i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[0][1]=='X'){
        ++count;
      }
  }
  for(int g = 0;g<=1;++g){
    if(mapArr[g][colCount-1]=='X'){
      ++count;
  }}
  for(int z = 0;z<=1;++z){
    if(mapArr[rowCount-1][z]=='X'){
      ++count;
    }
  }
  if(mapArr[rowCount-1][colCount-1]=='X'){
    ++count;
  }
  genHistory[0][2]=genHistory[0][1];
  genHistory[0][1]=genHistory[0][0];
  genHistory[0][0]=count;
  //Top right corner
  count = 0;
  for(int i = -1;i<=0;++i){
      if(mapArr[1][colCount-1+i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[0][colCount-2]=='X'){
        ++count;
      }
  }
  for(int g = 0;g<=1;++g){//left most collumn
    if(mapArr[g][0]=='X'){
      ++count;
  }}
  for(int z = -1;z<=0;++z){//bottom row
    if(mapArr[rowCount-1][colCount-1+z]=='X'){
      ++count;
    }
  }
  if(mapArr[rowCount-1][0]=='X'){//opposite corner
    ++count;
  }
  genHistory[colCount-1][2]=genHistory[colCount-1][1];
  genHistory[colCount-1][1]=genHistory[colCount-1][0];
  genHistory[colCount-1][0]=count;
  //Bottom left corner
  count = 0;
  for(int i = 0;i<=1;++i){
      if(mapArr[rowCount-2][i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[rowCount-1][1]=='X'){
        ++count;
      }
  }
  for(int g = 0;g<=1;++g){//top row
    if(mapArr[0][g]=='X'){
      ++count;
  }}
  for(int z = -1;z<=0;++z){//right collumn
    if(mapArr[rowCount-1+z][colCount-1]=='X'){
      ++count;
    }
  }
  if(mapArr[0][colCount-1]=='X'){//opposite corner
    ++count;
  }
  genHistory[totalCount-colCount][2]=genHistory[totalCount-colCount][1];
  genHistory[totalCount-colCount][1]=genHistory[totalCount-colCount][0];
  genHistory[totalCount-colCount][0]=count;
  //Bottom right corner
  count = 0;
  for(int i = -1;i<=0;++i){
      if(mapArr[rowCount-2][colCount-1+i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[rowCount-1][colCount-2]=='X'){
        ++count;
      }
  }
  for(int g = -1;g<=0;++g){//top row
    if(mapArr[0][colCount-1+g]=='X'){
      ++count;
  }}
  for(int z = -1;z<=0;++z){//left collumn
    if(mapArr[rowCount-1+z][0]=='X'){
      ++count;
    }
  }
  if(mapArr[0][0]=='X'){//opposite corner
    ++count;
  }
  genHistory[totalCount-1][2]=genHistory[totalCount-1][1];
  genHistory[totalCount-1][1]=genHistory[totalCount-1][0];
  genHistory[totalCount-1][0]=count;
  //First collumn except corners*
  for(int h = 1;h<rowCount-1;++h){
    count = 0;
    for(int i=0;i<=1;++i){
      if(mapArr[h-1][i]=='X'){
        ++count;
      }
      if(mapArr[h+1][i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[h][1]=='X'){
        ++count;
      }
    }
    for(int d = -1;d<=1;d++){ //check opposite collumn
      if(mapArr[h+d][colCount-1]=='X'){
        ++count;
      }
    }
    genHistory[colCount * h][2]=genHistory[colCount * h][1];
    genHistory[colCount * h][1]=genHistory[colCount * h][0];
    genHistory[colCount * h][0]=count;

  }
  //Last collumn except corners
  for(int h = 1;h<rowCount-1;++h){
    count = 0;
    for(int i=-1;i<=0;++i){
      if(mapArr[h-1][colCount-1+i]=='X'){
        ++count;
      }
      if(mapArr[h+1][colCount-1+i]=='X'){
        ++count;
      }
      if(i==0){
        continue;
      }
      if(mapArr[h][colCount-2]=='X'){
        ++count;
    }}
    for(int d = -1;d<=1;d++){ //check opposite collumn
      if(mapArr[h+d][0]=='X'){
        ++count;
      }
    }
    genHistory[colCount * (h+1)-1][2]=genHistory[colCount * (h+1)-1][1];
    genHistory[colCount * (h+1)-1][1]=genHistory[colCount * (h+1)-1][0];
    genHistory[colCount * (h+1)-1][0]=count;

  }
  //First row except corners
  for(int i = 1;i<colCount-1;++i){
    count = 0;
    for(int j = -1; j <= 1; ++j){
      if(mapArr[1][i+j]=='X'){
        ++count;
      }
      if(j==0){
        continue;
      }
      if(mapArr[0][i+j]=='X'){
        ++count;
      }
    }
    for(int z=-1;z<=1;z++){ //Check opposite row
      if(mapArr[rowCount-1][i+z]=='X'){
        ++count;
      }
    }
    genHistory[i][2]=genHistory[i][1];
    genHistory[i][1]=genHistory[i][0];
    genHistory[i][0]=count;
  }
  //Last row except corners
  for(int i = 1;i<colCount-1;++i){
    count = 0;
    for(int j = -1; j <= 1; ++j){
      if(mapArr[rowCount-2][i+j]=='X'){
        ++count;
      }
      if(j==0){
        continue;
      }
      if(mapArr[rowCount-1][i+j]=='X'){
        ++count;
      }
    }
    for(int z=-1;z<=1;z++){ //Check opposite row
      if(mapArr[0][i+z]=='X'){
        ++count;
      }
    }
    genHistory[((rowCount-1)*colCount)+i][2]=genHistory[(rowCount-1)*colCount+i][1];
    genHistory[(rowCount-1)*colCount+i][1]=genHistory[(rowCount-1)*colCount+i][0];
    genHistory[(rowCount-1)*colCount+i][0]=count;
  }
}

//Determine edge scores for Mirror mode
void cellMap::getMirrorNeighbors(){
  //Top left corner
  int count = 0;
  if(mapArr[0][0]=='X'){
    count+=3;
  }if(mapArr[1][0]=='X'){
    count+=2;
  }if(mapArr[0][1]=='X'){
    count+=2;
  }if(mapArr[1][1]=='X'){
    ++count;
  }
  genHistory[0][2]=genHistory[0][1];
  genHistory[0][1]=genHistory[0][0];
  genHistory[0][0]=count;
  //Top right corner
  count = 0;
  if(mapArr[0][colCount-1]=='X'){
    count+=3;
  }if(mapArr[1][colCount-1]=='X'){
    count+=2;
  }if(mapArr[0][colCount-2]=='X'){
    count+=2;
  }if(mapArr[1][colCount-2]=='X'){
    ++count;
  }
  genHistory[colCount-1][2]=genHistory[colCount-1][1];
  genHistory[colCount-1][1]=genHistory[colCount-1][0];
  genHistory[colCount-1][0]=count;
  //Bottom left corner
  count = 0;
  if(mapArr[rowCount-1][0]=='X'){
    count+=3;
  }if(mapArr[rowCount-2][0]=='X'){
    count+=2;
  }if(mapArr[rowCount-1][1]=='X'){
    count+=2;
  }if(mapArr[rowCount-2][1]=='X'){
    ++count;
  }
  genHistory[totalCount-colCount][2]=genHistory[totalCount-colCount][1];
  genHistory[totalCount-colCount][1]=genHistory[totalCount-colCount][0];
  genHistory[totalCount-colCount][0]=count;
  //Bottom right corner
  count = 0;
  if(mapArr[rowCount-1][colCount-1]=='X'){
    count+=3;
  }if(mapArr[rowCount-2][colCount-1]=='X'){
    count+=2;
  }if(mapArr[rowCount-1][colCount-2]=='X'){
    count+=2;
  }if(mapArr[rowCount-2][colCount-2]=='X'){
    ++count;
  }
  genHistory[totalCount-1][2]=genHistory[totalCount-1][1];
  genHistory[totalCount-1][1]=genHistory[totalCount-1][0];
  genHistory[totalCount-1][0]=count;
  //First collumn except corners
  for(int h = 1;h<rowCount-1;++h){
    count = 0;
    for(int i=0;i<=1;++i){
      if(mapArr[h-1][i]=='X'){
        ++count;
        if(i==0){
          ++count;
        }
      }
      if(mapArr[h+1][i]=='X'){
        ++count;
        if(i==0){
          ++count;
        }
      }
      if(i==0){
        continue;
      }
      if(mapArr[h][1]=='X'){
        ++count;
      }
    }
    if(mapArr[h][0]=='X'){
      ++count;
    }
    genHistory[colCount * h][2]=genHistory[colCount * h][1];
    genHistory[colCount * h][1]=genHistory[colCount * h][0];
    genHistory[colCount * h][0]=count;

  }
  //Last collumn except corners
  for(int h = 1;h<rowCount-1;++h){
    count = 0;
    for(int i=-1;i<=0;++i){
      if(mapArr[h-1][colCount-1+i]=='X'){
        ++count;
        if(i==0){
          ++count;
        }
      }
      if(mapArr[h+1][colCount-1+i]=='X'){
        ++count;
        if(i==0){
          ++count;
        }
      }
      if(i==0){
        continue;
      }
      if(mapArr[h][colCount-2]=='X'){
        ++count;
      }
    }
    if(mapArr[h][colCount-1]=='X'){
      ++count;
    }
    genHistory[colCount * (h+1)-1][2]=genHistory[colCount * (h+1)-1][1];
    genHistory[colCount * (h+1)-1][1]=genHistory[colCount * (h+1)-1][0];
    genHistory[colCount * (h+1)-1][0]=count;

  }
  //First row except corners
  for(int i = 1;i<colCount-1;++i){
    count = 0;
    for(int j = -1; j <= 1; ++j){
      if(mapArr[1][i+j]=='X'){
        ++count;
      }
      if(mapArr[0][i+j]=='X'){
        if(j==0){
          ++count;
        }else{
          count+=2;
        }
      }
    }
    genHistory[i][2]=genHistory[i][1];
    genHistory[i][1]=genHistory[i][0];
    genHistory[i][0]=count;
  }
  //Last row except corners
  for(int i = 1;i<colCount-1;++i){
    count = 0;
    for(int j = -1; j <= 1; ++j){
      if(mapArr[rowCount-2][i+j]=='X'){
        ++count;
      }
      if(mapArr[rowCount-1][i+j]=='X'){
        if(j==0){
          ++count;
        }else{
          count+=2;
        }
      }
    }
    genHistory[((rowCount-1)*colCount)+i][2]=genHistory[(rowCount-1)*colCount+i][1];
    genHistory[(rowCount-1)*colCount+i][1]=genHistory[(rowCount-1)*colCount+i][0];
    genHistory[(rowCount-1)*colCount+i][0]=count;
  }
}
