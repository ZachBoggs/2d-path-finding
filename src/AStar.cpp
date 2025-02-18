#include "raylib.h"
#include "AStar.h"
#include "mazeSolvingAlgorithm.h"
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using std::vector;
using std::string;




int getDistanceMeasure(Vector2 lhs)
{
  //gets the smallest square
  
  Vector2 absVals = {abs(lhs.x),abs(lhs.y)};

  int extraLength(0);
  int totalLength(0);
  if(absVals.x < absVals.y)
  {
    //             diag dist covered      remainding distance
    totalLength = (14 * absVals.x) + (10 * (absVals.y - absVals.x));
  }else
  {
    totalLength = (14 * absVals.y) + (10 * (absVals.x - absVals.y));
  }

  return totalLength;
}


/*
 *
 * Vector4 x & y represents the difference between the current cell and the starting cell
 * Vector4 z & w represents the difference between the current cell and the ending cell
 *
 */
bool operator<(const Vector4& lhs, const Vector4& rhs)
{
  //Vector2 lhsDiff{lhs.x - tileGrid.currentEnd.x,lhs.y - tileGrid.currentEnd.y};
  //Vector2 rhsDiff{rhs.x - tileGrid.currentEnd.x,rhs.y - tileGrid.currentEnd.y};

  int lhsStartSmall = getDistanceMeasure(Vector2{lhs.x,lhs.y});
  int lhsEndSmall   = getDistanceMeasure(Vector2{lhs.z,lhs.w});
  
  int rhsStartSmall = getDistanceMeasure(Vector2{rhs.x,rhs.y});
  int rhsEndSmall   = getDistanceMeasure(Vector2{rhs.z,rhs.w});

  //return (lhsStartSmall + lhsEndSmall) < (rhsStartSmall + rhsEndSmall);
  return (lhsStartSmall + lhsEndSmall) > (rhsStartSmall + rhsEndSmall);
}

AStar::AStar(const grid& inputGrid) 
  : mazeSolvingAlgorithm("AStar",vector<Vector2>{{-1,0},{0,1},{1,0},{0,-1},{1,-1},{1,1},{-1,-1},{-1,1}})
  //: mazeSolvingAlgorithm("AStar",vector<Vector2>{{-1,0},{0,1},{1,0},{0,-1}})
{
  tileGrid = inputGrid;
  loadedMaze = true;
  //cellStack.push_back(tileGrid.currentStart);
  cellStack.push_back(Vector4{0.0f,0.0f,tileGrid.currentEnd.x - tileGrid.currentStart.x,tileGrid.currentEnd.y - tileGrid.currentStart.y});

}

void AStar::step() 
{
  if(!loadedMaze) { return; }
  if(foundShortestPath) { return; }

  // marking the current cell in the grid and removing it from the queue
  if(cellStack.size() == 0) { return; }


  std::pop_heap(cellStack.begin(),cellStack.end());
  Vector4 currentLocation = cellStack.back();
  cellStack.pop_back();

  Vector2 cellCoord = tileGrid.currentStart;
  cellCoord.x += currentLocation.x;
  cellCoord.y += currentLocation.y;
  //cellStack.erase(cellStack.begin());

  
  //std::cout << "stepping in AStar" << std::endl;
  bool addedToPath(false);
  tileGrid.tiles[cellCoord.y][cellCoord.x].visit();
  for(const Vector2& pattern : searchPattern)
  {
    int currentY = (int)cellCoord.y + (int)pattern.y;
    int currentX = (int)cellCoord.x + (int)pattern.x;

    // making sure that we are in bounds where we would be searching
    if(currentX < 0 || currentX >= tileGrid.cellCountX) { continue; }
    if(currentY < 0 || currentY >= tileGrid.cellCountY) { continue; }

    if(tileGrid.tiles[currentY][currentX].isEnd())
    {
      tileGrid.tiles[currentY][currentX].see(cellCoord);
      tileGrid.tiles[currentY][currentX].setDistance(tileGrid.tiles[cellCoord.y][cellCoord.x].getDistance() + 1);
      endPlacements.push_back(Vector2{(float)currentX,(float)currentY});
      foundShortestPath = true;
      addedToPath       = false;

      endPlacements.push_back(Vector2{cellCoord.x,cellCoord.y});
      for(int i =0;i<cellStack.size();++i)
      {
        currentLocation = cellStack[i];
        Vector2 currentCoord = tileGrid.currentStart;

        currentCoord.x += currentLocation.x;
        currentCoord.y += currentLocation.y;

        endPlacements.push_back(currentCoord);
      }
        
      std::cout << "computing the shortest path" << std::endl;
      computeSearchPath();
      std::cout << "done computing the shortest path" << std::endl;
      return;
    }else if(tileGrid.tiles[currentY][currentX].isWall())
    {
      continue;
    }else if(!tileGrid.tiles[currentY][currentX].isSeen())
    {
      addedToPath = true;
      tileGrid.tiles[currentY][currentX].see(cellCoord);

      //std::cout << "currentX " << currentX << ", currentY " << currentY << std::endl;
      //cellStack.push_back(Vector2{(float)currentX,(float)currentY});
      cellStack.push_back
      (
        Vector4
        {
          (float)(currentX-tileGrid.currentStart.x),
          (float)(currentY-tileGrid.currentStart.y),
          (float)(currentX-tileGrid.currentEnd.x),
          (float)(currentY-tileGrid.currentEnd.y)
        }
      );
      std::push_heap(cellStack.begin(),cellStack.end());
      tileGrid.tiles[currentY][currentX].setDistance(tileGrid.tiles[cellCoord.y][cellCoord.x].getDistance() + 1);
    }
  }

  if(!addedToPath)
  {
    endPlacements.push_back(Vector2{cellCoord.x,cellCoord.y});
    computeSearchPath();
  }
}

bool AStar::hasRule() { return false; }


