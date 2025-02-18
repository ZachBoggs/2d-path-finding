#include "raylib.h"
#include "dfsUpdated.h"
#include "mazeSolvingAlgorithm.h"
#include "grid.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;


dfsUpdated::dfsUpdated(const grid& inputGrid) 
  : mazeSolvingAlgorithm
    (
      "dfsUpdated",
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
  orderHolder[0] = &dfsUpdated::goUp;
  orderHolder[1] = &dfsUpdated::goLeft;
  orderHolder[2] = &dfsUpdated::goDown;
  orderHolder[3] = &dfsUpdated::goRight;
  */

  //(this->*orderHolder[0])();

  tileGrid.tiles[currentLocation.y][currentLocation.x].setFoundingCell(Vector2{-1.0f,-1.0f});
}

void dfsUpdated::step() 
{
  //std::cout << "dfsUpdated step" << std::endl;
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
    std::cout << "found end path in dfsUpdated" << std::endl;
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

    if (shiftedCell.isVisited() && (shiftedCell.getDistance() <= tileGrid.tiles[currentLocation.y][currentLocation.x].getDistance() + 1) )
    {
      continue;
    }
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
    bool foundValidSpot(false);

    while(!foundValidSpot)
    {
      for(int i=0;i<directionOrder.size();++i)
      {

        Vector2 newLoc = Vector2{currentLocation.x + searchPattern[directionOrder[i]].x,currentLocation.y + searchPattern[directionOrder[i]].y};

        if(newLoc.x < 0 || newLoc.x >= tileGrid.cellCountX) { continue; }
        if(newLoc.y < 0 || newLoc.y >= tileGrid.cellCountY) { continue; }

        cell& shiftedCell = tileGrid.tiles[newLoc.y][newLoc.x];

        if (shiftedCell.isWall() ) { continue; }
        if(shiftedCell.isVisited() && shiftedCell.getDistance() < tileGrid.tiles[currentLocation.y][currentLocation.x].getDistance() + 1) 
        { 
          continue; 
        }


        foundValidSpot = true;
        break;

        

        //check all directions if we can move there, if we can't then we don't flip the flag and we continue back
        //we stop going back if we see that the current location has a founding cell location of (-1,-1)



      }
      currentLocation = tileGrid.tiles[currentLocation.y][currentLocation.x].getFoundingCell();
    }

  }

  //in this situation, we traverse back until we can find a location that we can expand from
}

bool dfsUpdated::hasRule() 
{
  return true;
}

//have it so the cells have a distance variable that can be set, by default the distance is -1 in the constructor

//have it so when the compute path function is ran, it also checks the distance measure on the cell in comparision to the neighbords instead of the see cell, if the path is shorter, then it goes that way all the way back to the start
