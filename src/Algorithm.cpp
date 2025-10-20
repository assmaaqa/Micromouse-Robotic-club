
#include <vector>
#include <utility>
#include <queue>
using namespace std;

#include "Algorithm.h"
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;


int cellsArray[6][6] = { -1 };  //put this to -1 as (unvisited), if visited and no walls put to 0

int floodArray[6][6] = {
  { 4, 3, 2, 2, 3, 4},
  { 3, 2, 1, 1, 2, 3},
  { 2, 1, 0, 0, 1, 2},
  { 2, 1, 0, 0, 1, 2},
  { 3, 2, 1, 1, 2, 3},
  { 4, 3, 2, 2, 3, 4},
};
void updateCells(int x, int y, int orient) {
  bool l = wallRight(), r = wallLeft(), f = wallFront();
  if (l && r && f) {
    if (orient == NORTH) cellsArray[x][y] = 13;
    else if (orient == EAST) cellsArray[x][y] = 14;
    else if (orient == SOUTH) cellsArray[x][y] = 15;
    else if (orient == WEST) cellsArray[x][y] = 16;
  } else if (l && r && !f) {
    if (orient == NORTH || orient == SOUTH) cellsArray[x][y] = 9;
    else cellsArray[x][y] = 10;
  } else if (l && !r && f) {
    if (orient == NORTH) cellsArray[x][y] = 8;
    else if (orient == EAST) cellsArray[x][y] = 7;
    else if (orient == SOUTH) cellsArray[x][y] = 6;
    else if (orient == WEST) cellsArray[x][y] = 5;
  } else if (!l && r && f) {
    if (orient == NORTH) cellsArray[x][y] = 7;
    else if (orient == EAST) cellsArray[x][y] = 6;
    else if (orient == SOUTH) cellsArray[x][y] = 5;
    else if (orient == WEST) cellsArray[x][y] = 8;
  } else if (f) {
    if (orient == NORTH) cellsArray[x][y] = 2;
    else if (orient == EAST) cellsArray[x][y] = 3;
    else if (orient == SOUTH) cellsArray[x][y] = 4;
    else if (orient == WEST) cellsArray[x][y] = 1;
  } else if (l) {
    if (orient == NORTH) cellsArray[x][y] = 1;
    else if (orient == EAST) cellsArray[x][y] = 2;
    else if (orient == SOUTH) cellsArray[x][y] = 3;
    else if (orient == WEST) cellsArray[x][y] = 4;
  } else if (r) {
    if (orient == NORTH) cellsArray[x][y] = 3;
    else if (orient == EAST) cellsArray[x][y] = 4;
    else if (orient == SOUTH) cellsArray[x][y] = 1;
    else if (orient == WEST) cellsArray[x][y] = 2;
  } else if (!r && !f && !l) cellsArray[x][y] = 0;
}


bool isAccessible(int current_x, int current_y, int target_x, int target_y) {
  if (current_x == target_x) {  // if the target is below current
    if (current_y > target_y) {
      if ((cellsArray[current_x][current_y] == 4) || (cellsArray[current_x][current_y] == 5) || (cellsArray[current_x][current_y] == 6) || (cellsArray[current_x][current_y] == 10) || (cellsArray[current_x][current_y] == 16) || (cellsArray[current_x][current_y] == 15) || (cellsArray[current_x][current_y] == 14))
        return false;
      else
        return true;
    } else {  // if the target is above current
      if ((cellsArray[current_x][current_y] == 2) || (cellsArray[current_x][current_y] == 7) || (cellsArray[current_x][current_y] == 8) || (cellsArray[current_x][current_y] == 10) || (cellsArray[current_x][current_y] == 16) || (cellsArray[current_x][current_y] == 13) || (cellsArray[current_x][current_y] == 14))
        return false;
      else
        return true;
    }
  } else if (current_y == target_y) {  // if target is on the left of the current
    if (current_x > target_x) {
      if ((cellsArray[current_x][current_y] == 1) || (cellsArray[current_x][current_y] == 5) || (cellsArray[current_x][current_y] == 8) || (cellsArray[current_x][current_y] == 9) || (cellsArray[current_x][current_y] == 16) || (cellsArray[current_x][current_y] == 13) || (cellsArray[current_x][current_y] == 15))
        return false;
      else
        return true;
    } else {  // if target is on the right of the current
      if ((cellsArray[current_x][current_y] == 3) || (cellsArray[current_x][current_y] == 6) || (cellsArray[current_x][current_y] == 7) || (cellsArray[current_x][current_y] == 9) || (cellsArray[current_x][current_y] == 15) || (cellsArray[current_x][current_y] == 14) || (cellsArray[current_x][current_y] == 13))
        return false;
      else
        return true;
    }
  }
}


int orientation(int orient, char turning) {
  if (turning == 'L') {
    orient -= 1;
    if (orient == -1)
      orient = 3;
  } else if (turning == 'R') {
    orient += 1;
    if (orient == 4)
      orient = 0;
  } else if (turning == 'B') {
    if (orient == 0)
      orient = 2;
    else if (orient == 1)
      orient = 3;
    else if (orient == 2)
      orient = 0;
    else if (orient == 3)
      orient = 1;
  }
  return orient;
}


vector<pair<int, int>> getSurrounding(int x, int y) {
  vector<pair<int, int>> surrounding(4);
  if (x + 1 >= 6) {
    surrounding[1].first = -1;
  } else surrounding[1].first = x + 1;
  surrounding[1].second = y;

  surrounding[0].first = x;
  if (y + 1 >= 6) {
    surrounding[0].second = -1;
  } else surrounding[0].second = y + 1;

  surrounding[2].first = x;
  surrounding[2].second = y - 1;

  surrounding[3].first = x - 1;
  surrounding[3].second = y;

  return surrounding;
}


char whereToMoveWithDFS(int x, int y, int prex, int prey, int orient) {
  vector<pair<int, int>> surrounding = getSurrounding(x, y);
  pair<int, int> north = surrounding[0];
  pair<int, int> east = surrounding[1];
  pair<int, int> south = surrounding[2];
  pair<int, int> west = surrounding[3];

  int minValue = 1000;  // Initialize with a high value
  int minCell = 1000;   // Initialize to an invalid value
  int previous = -1;
  int AccessiblePathsNum = 0;

  int accessibleNeighbors[4] = { 1000, 1000, 1000, 1000 };

  if (north.first == prex && north.second == prey) previous = NORTH;
  else if (east.first == prex && east.second == prey) previous = EAST;
  else if (south.first == prex && south.second == prey) previous = SOUTH;
  else previous = WEST;
  bool b = false;
  for (int i = 0; i < 4; i++) {
    if (surrounding[i].first >= 0 && surrounding[i].second >= 0 && isAccessible(x, y, surrounding[i].first, surrounding[i].second)) {
      if (cellsArray[surrounding[i].first][surrounding[i].second] == -1) {
        b = true;
        accessibleNeighbors[i] = floodArray[surrounding[i].first][surrounding[i].second];
        AccessiblePathsNum++;
      }
    }
  }
  if (!b) {
    for (int i = 0; i < 4; i++) {
      if (surrounding[i].first >= 0 && surrounding[i].second >= 0 && isAccessible(x, y, surrounding[i].first, surrounding[i].second)) {
        accessibleNeighbors[i] = floodArray[surrounding[i].first][surrounding[i].second];
        AccessiblePathsNum++;
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    if (accessibleNeighbors[i] != 1000 && accessibleNeighbors[i] < minValue) {
      if (AccessiblePathsNum == 1) {
        minValue = accessibleNeighbors[i];
        minCell = i;
      } else {
        if (i == previous) {
          continue;
        } else {
          minValue = accessibleNeighbors[i];
          minCell = i;
        }
      }
    }
  }
  if (minCell == orient)
    return 'F';
  else if ((minCell == (orient - 1) % 4) || (minCell == (orient + 3) % 4))
    return 'L';
  else if ((minCell == (orient + 1) % 4) || (minCell == (orient - 3) % 4))
    return 'R';
  else
    return 'B';
}


bool isIncrementConsistent(int x, int y) {
  vector<pair<int, int>> surrounding = getSurrounding(x, y);
  pair<int, int> north = surrounding[0];
  pair<int, int> east = surrounding[1];
  pair<int, int> south = surrounding[2];
  pair<int, int> west = surrounding[3];
  int curentValue = floodArray[x][y];
  int minValues[4] = { 1000, 1000, 1000, 1000 };
  int minCounts = 0;

  if ((north.first >= 0) && (north.second >= 0) && isAccessible(x, y, north.first, north.second))
    minValues[NORTH] = floodArray[north.first][north.second];
  if ((east.first >= 0) && (east.second >= 0) && isAccessible(x, y, east.first, east.second))
    minValues[EAST] = floodArray[east.first][east.second];
  if ((south.first >= 0) && (south.second >= 0) && isAccessible(x, y, south.first, south.second))
    minValues[SOUTH] = floodArray[south.first][south.second];
  if ((west.first >= 0) && (west.second >= 0) && isAccessible(x, y, west.first, west.second))
    minValues[WEST] = floodArray[west.first][west.second];

  for (int i = 0; i < 4; i++) {
    if ((minValues[i] == (curentValue - 1)) && (minValues[i] != 1000))
      minCounts++;
  }

  if (minCounts > 0)
    return true;
  else
    return false;
}

void makeCellConsistent(int x, int y) {
  vector<pair<int, int>> surrounding = getSurrounding(x, y);
  pair<int, int> north = surrounding[0];
  pair<int, int> east = surrounding[1];
  pair<int, int> south = surrounding[2];
  pair<int, int> west = surrounding[3];

  int currentValue = floodArray[x][y];
  int minValues[4] = { 1000, 1000, 1000, 1000 };  // Initialize with a high value.

  if (north.first >= 0 && north.second >= 0 && isAccessible(x, y, north.first, north.second))
    minValues[NORTH] = floodArray[north.first][north.second];
  if (east.first >= 0 && east.second >= 0 && isAccessible(x, y, east.first, east.second))
    minValues[EAST] = floodArray[east.first][east.second];
  if (south.first >= 0 && south.second >= 0 && isAccessible(x, y, south.first, south.second))
    minValues[SOUTH] = floodArray[south.first][south.second];
  if (west.first >= 0 && west.second >= 0 && isAccessible(x, y, west.first, west.second))
    minValues[WEST] = floodArray[west.first][west.second];

  int minimalValue = 1000;

  for (int i = 0; i < 4; i++) {
    if (minValues[i] < minimalValue)
      minimalValue = minValues[i];
  }


  if (minimalValue != 1000) {
    floodArray[x][y] = minimalValue + 1;
  }
}

bool visited[16][16] = { false };  // Initialize a visited array to keep track of visited cells

void floodFillUsingQueue(int start_x, int start_y, int previous_x, int previous_y) {

  std::queue<int> cellQueue;

  if (!isIncrementConsistent(start_x, start_y))
    floodArray[start_x][start_y] = floodArray[start_x][start_y] + 1;


  cellQueue.push(start_x);
  cellQueue.push(start_y);

  while (!cellQueue.empty()) {
    int current_X = cellQueue.front();
    cellQueue.pop();
    int current_Y = cellQueue.front();
    cellQueue.pop();

    // Check if the current cell is already visited
    if (visited[current_Y][current_X]) {
      continue;  // Skip this cell
    }

    // Mark the current cell as visited
    visited[current_Y][current_X] = true;

    // Check if the current cell is incrementally consistent with its neighbors
    if (!isIncrementConsistent(current_X, current_Y)) {
      // If not consistent, make it consistent and increment its value
      makeCellConsistent(current_X, current_Y);

      vector<pair<int, int>> surrounding = getSurrounding(current_X, current_Y);
      pair<int, int> north = surrounding[0];
      pair<int, int> east = surrounding[1];
      pair<int, int> south = surrounding[2];
      pair<int, int> west = surrounding[3];


      int neighborsX[] = { north.first, east.first, south.first, west.first };
      int neighborsY[] = { north.second, east.second, south.second, west.second };

      for (int i = 0; i < 4; i++) {
        int neighborX = neighborsX[i];
        int neighborY = neighborsY[i];

        if (neighborX >= 0 && neighborY >= 0 && isAccessible(current_X, current_Y, neighborX, neighborY)) {
          cellQueue.push(neighborX);
          cellQueue.push(neighborY);
        }
      }
    } else {
    }
  }
}

void updateCoordinates(int orient, int *new_x, int *new_y)  //****تعديل*****
{

  if (orient == 0)
    *new_y += 1;
  else if (orient == 1)
    *new_x += 1;
  else if (orient == 2)
    *new_y -= 1;
  else if (orient == 3)
    *new_x -= 1;
}

int current_x = 0, current_y = 0, previous_x = 0, previous_y = 0, orient = NORTH;
char Direction = 0;


// Put your implementation of floodfill here!
void floodFill() {
  int left = 0, right = 0, forward = 0;
  left = wallLeft();
  right = wallRight();
  forward = wallFront();

  updateCells(current_x, current_y, orient);//*
  if (floodArray[current_x][current_y] != 0) {
    floodFillUsingQueue(current_x, current_y, previous_x, previous_y);//* 
  } else {
    while (true);
  }
  Direction = whereToMoveWithDFS(current_x, current_y, previous_x, previous_y, orient);//* delete
  //* move to less flood fill
  if (Direction == 'L') {
    turn(0);  //left
    orient = orientation(orient, 'L');
  } else if (Direction == 'R') {
    turn(1);  //right
    orient = orientation(orient, 'R');
  } else if (Direction == 'B') {
    turn(0);  //left
    orient = orientation(orient, 'L');
    turn(0);  //left
    orient = orientation(orient, 'L');
  }

  moveForward();  //pid function
  previous_x = current_x;
  previous_y = current_y;

  updateCoordinates(orient, &current_x, &current_y);
}

void afterExploration(){
  int left = 0, right = 0, forward = 0;
  left = wallLeft();
  right = wallRight();
  forward = wallFront();

  if(floodArray[current_x][current_y] == 0) {
    while (true);
  }

  


}


char moveToMin(int x, int y, int prex, int prey, int orient) {
  int compare=floodArray[x][y]-1;


  vector<pair<int, int>> surrounding = getSurrounding(x, y);
  pair<int, int> north = surrounding[0];
  pair<int, int> east = surrounding[1];
  pair<int, int> south = surrounding[2];
  pair<int, int> west = surrounding[3];

  int minValue = 1000;  // Initialize with a high value
  int minCell = 1000;   // Initialize to an invalid value
  int previous = -1;
  int AccessiblePathsNum = 0;

  int accessibleNeighbors[4] = { 1000, 1000, 1000, 1000 };

  if (north.first == prex && north.second == prey) previous = NORTH;
  else if (east.first == prex && east.second == prey) previous = EAST;
  else if (south.first == prex && south.second == prey) previous = SOUTH;
  else previous = WEST;

  for (int i = 0; i < 4; i++) {
    if (surrounding[i].first >= 0 && surrounding[i].second >= 0 && isAccessible(x, y, surrounding[i].first, surrounding[i].second)) {
        accessibleNeighbors[i] = floodArray[surrounding[i].first][surrounding[i].second];
        AccessiblePathsNum++;
    }
  }
  
  for (int i = 0; i < 4; i++) {
    if (accessibleNeighbors[i] != 1000 && accessibleNeighbors[i] == compare) {
      if(i==previous) continue;
      minCell=i;
    }
  }
  if (minCell == orient)
    return 'F';
  else if ((minCell == (orient - 1) % 4) || (minCell == (orient + 3) % 4))
    return 'L';
  else if ((minCell == (orient + 1) % 4) || (minCell == (orient - 3) % 4))
    return 'R';
  else
    return 'B';
}