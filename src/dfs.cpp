#include "raylib.h"
#include "dfs.h"
#include "mazeSolvingAlgorithm.h"
#include "grid.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

typedef bool (dfs::*directionFunc)();

dfs::dfs(const grid& inputGrid) 
  : mazeSolvingAlgorithm
    (
      "dfs",
    //directionOrder{0,2,4,6,1,3,5,7},
    //directionOrder{1,3,5,7,0,2,4,6}, 
      vector<Vector2>
      {
        {-1.0f, 0.0f},
        { 0.0f, 1.0f},
        { 1.0f, 0.0f},
        { 0.0f,-1.0f},
        { 1.0f,-1.0f},
        { 1.0f, 1.0f},
        {-1.0f, 1.0f},
        {-1.0f,-1.0f}
      }
    ),
  directionOrder{0,1,2,3,4,5,6,7}
{
  tileGrid = inputGrid;
  loadedMaze = true;

  currentLocation = tileGrid.currentStart;
  // set starting location here
  
  /*
  orderHolder[0] = &dfs::goUp;
  orderHolder[1] = &dfs::goLeft;
  orderHolder[2] = &dfs::goDown;
  orderHolder[3] = &dfs::goRight;
  */

  //(this->*orderHolder[0])();

  tileGrid.tiles[currentLocation.y][currentLocation.x].setFoundingCell(Vector2{-1.0f,-1.0f});
}

void dfs::step() 
{
  //std::cout << "dfs step" << std::endl;
  if(!loadedMaze) { return; }
  if(foundShortestPath) { return; }

  // if our current location is (-1,-1) because we have searched all that we can
  if(currentLocation.y < 0 || currentLocation.x < 0) { return; }

  //tileGrid.tiles[currentLocation.y][currentLocation.x].visit();

  // if we found the end point
  if(tileGrid.tiles[currentLocation.y][currentLocation.x].isEnd())
  {
    foundShortestPath = true;

    computeSearchPath();
    std::cout << "found end path in dfs" << std::endl;
    endPlacements.push_back(currentLocation);
    return;
  }

  bool moved(false);
  for(int i=0;i<searchPattern.size();++i)
  {
    Vector2 newLoc = Vector2{currentLocation.x + searchPattern[directionOrder[i]].x,currentLocation.y + searchPattern[directionOrder[i]].y};
    if(newLoc.x < 0 || newLoc.x >= tileGrid.cellCountX) { continue; }
    if(newLoc.y < 0 || newLoc.y >= tileGrid.cellCountY) { continue; }

    cell& shiftedCell = tileGrid.tiles[newLoc.y][newLoc.x];

    //cell& shiftedCell = tileGrid.tiles[currentLocation.y + searchPattern[directionOrder[i]].y][currentLocation.x + searchPattern[directionOrder[i]].x];

    //if (shiftedCell.isWall() || shiftedCell.isVisited() && !shiftedCell.isEnd()) 
    if (shiftedCell.isWall() && !shiftedCell.isEnd()) { continue; }

    if (shiftedCell.isVisited()) { continue; }
    moved = true;

    //tileGrid.tiles[currentLocation.y][currentLocation.x].visit();
    //tileGrid.tiles[currentLocation.y + 1][currentLocation.x].see(currentLocation);

    //std::cout << "(" << currentLocation.x << ", " << currentLocation.y << ")";
    //std::cout << " -- see -> (" << newLoc.x << ", " << newLoc.y << ")" << std::endl;
    shiftedCell.see(currentLocation);
    shiftedCell.setDistance(tileGrid.tiles[currentLocation.y][currentLocation.x].getDistance() + 1);
    currentLocation = newLoc;
    tileGrid.tiles[currentLocation.y][currentLocation.x].visit();
    break;
    //currentLocation.y++;
    //std::cout << "can't move in direction [" << i << "]" << std::endl;
  }

  // backtracking if we couldn't move anywhere
  if(!moved) 
  { 
    endPlacements.push_back(currentLocation); 

    Vector2 oldLoc = currentLocation;
    currentLocation = tileGrid.tiles[currentLocation.y][currentLocation.x].getFoundingCell();
    //std::cout << "(" << currentLocation.x << ", " << currentLocation.y << ")";
    //std::cout << " <- back -- (" << oldLoc.x << ", " << oldLoc.y << ")" << std::endl;
  }

  //in this situation, we traverse back until we can find a location that we can expand from
}

bool dfs::hasRule() 
{
  return true;
}

//have it so the cells have a distance variable that can be set, by default the distance is -1 in the constructor

//have it so when the compute path function is ran, it also checks the distance measure on the cell in comparision to the neighbords instead of the see cell, if the path is shorter, then it goes that way all the way back to the start
