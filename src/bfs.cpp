#include "raylib.h"
#include "bfs.h"
#include "mazeSolvingAlgorithm.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

bfs::bfs(const grid& inputGrid) 
  : mazeSolvingAlgorithm("bfs",vector<Vector2>{{-1,0},{0,1},{1,0},{0,-1},{1,-1},{1,1},{-1,-1},{-1,1}})
  //: mazeSolvingAlgorithm("bfs",vector<Vector2>{{-1,0},{0,1},{1,0},{0,-1}})
{
  tileGrid = inputGrid;
  loadedMaze = true;
  cellStack.push_back(tileGrid.currentStart);

}

void bfs::step() 
{
  if(!loadedMaze) { return; }
  if(foundShortestPath) { return; }

  // marking the current cell in the grid and removing it from the queue
  if(cellStack.size() == 0) { return; }

  Vector2 cellCoord = cellStack[0];
  cellStack.erase(cellStack.begin());

  
  //std::cout << "stepping in bfs" << std::endl;
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
        endPlacements.push_back(cellStack[i]);
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
      //the cells founding coordinates are being not initialized in some instances, like the root
      //tileGrid.tiles[currentY][currentX].setFoundingCell(cellCoord);


      //std::cout << "currentX " << currentX << ", currentY " << currentY << std::endl;
      cellStack.push_back(Vector2{(float)currentX,(float)currentY});
      tileGrid.tiles[currentY][currentX].setDistance(tileGrid.tiles[cellCoord.y][cellCoord.x].getDistance() + 1);
      //std::cout << "pushing to bfs stack" << std::endl;
    }
  }

  // if we didn't add any more nodes to the cellStack, then we want to add the coordinate to the stack for backtracking
  // we also want to mark the cell for later that the cell has alreadu been baktracked from so then we don't add more then 1 of the current paths to the search path vector4
  if(!addedToPath)
  {
    endPlacements.push_back(Vector2{cellCoord.x,cellCoord.y});
    computeSearchPath();
  }



  // if we have found the end coordinate then we set the path to the search path
}

bool bfs::hasRule() { return false; }


