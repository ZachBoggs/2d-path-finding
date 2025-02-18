#include <vector>
#include <string>
#include <iostream>
#include "raylib.h"
#include "mazeSolvingAlgorithm.h"
#include "grid.h"

using std::vector;
using std::string;

void mazeSolvingAlgorithm::display()
{
  if(loadedMaze)
  {
    tileGrid.draw(Vector2{-1.0f,-1.0f});
    drawSearchPath(RED);
  }
}

void mazeSolvingAlgorithm::resizeToWindow(float newX,float newY,float newWidth,float newHeight)
{
  tileGrid.resize(newX,newY,newWidth,newHeight);
}



void mazeSolvingAlgorithm::drawSearchPath(Color lineColor)
{

  Rectangle refCell = tileGrid.tiles[0][0].getCellBox();
  float cellWidth = refCell.width;
  Vector2 startingCoordinate = Vector2{refCell.x + (cellWidth/2),refCell.y + (cellWidth/2)};

  for(int i=0;i<searchPath.size();++i)
  {
    DrawLineEx
    (
      Vector2
      {
        startingCoordinate.x + (searchPath[i].y * cellWidth),
        startingCoordinate.y + (searchPath[i].x * cellWidth)
      },
      Vector2
      {
        startingCoordinate.x + (searchPath[i].w * cellWidth),
        startingCoordinate.y + (searchPath[i].z * cellWidth)
      },
      2,
      Color
      {
        (unsigned char)(255 - (int)((255.0f / (float)searchPath.size()) * i)),
        (unsigned char)((255.0f / (float)searchPath.size()) * i),
        0,
        255
      }
    );
  }

  if(!foundShortestPath) { return; }

  for(int i=0;i<directPath.size();++i)
  {
    DrawLineEx
    (
      Vector2
      {
        startingCoordinate.x + (directPath[i].y * cellWidth),
        startingCoordinate.y + (directPath[i].x * cellWidth)
      },
      Vector2
      {
        startingCoordinate.x + (directPath[i].w * cellWidth),
        startingCoordinate.y + (directPath[i].z * cellWidth)
      },
      3,
      ORANGE
    );
  }

  /*
  for(int i=0;i<shortestPath.size();++i)
  {
    DrawLineEx
    (
      Vector2
      {
        startingCoordinate.x + (shortestPath[i].y * cellWidth),
        startingCoordinate.y + (shortestPath[i].x * cellWidth)
      },
      Vector2
      {
        startingCoordinate.x + (shortestPath[i].w * cellWidth),
        startingCoordinate.y + (shortestPath[i].z * cellWidth)
      },
      3,
      YELLOW
    );
  }
  */
}

mazeSolvingAlgorithm::mazeSolvingAlgorithm(string algName,vector<Vector2> newSearchPattern) 
  : algorithmName(algName),loadedMaze(false),foundShortestPath(false),shortestPath(),directPath(),searchPath(),searchPattern(newSearchPattern)
{
}

vector<Vector4> mazeSolvingAlgorithm::wanderedPathFromCell(Vector2 currentLocation,grid& inputGrid, const bool& ignorePreviousPaths)
{
  vector<Vector4> path;
  
  Vector2 tempLoc;
  if(currentLocation.x < 0 || currentLocation.y < 0) { return path; }
  while (!inputGrid.tiles[currentLocation.y][currentLocation.x].isStart())
  {
    tempLoc = inputGrid.tiles[currentLocation.y][currentLocation.x].getFoundingCell();
    
    if(inputGrid.tiles[currentLocation.y][currentLocation.x].usedInPath() && !ignorePreviousPaths) { break; }

    inputGrid.tiles[currentLocation.y][currentLocation.x].setUsedInPath();


    
    path.push_back(Vector4{currentLocation.x,currentLocation.y,tempLoc.x,tempLoc.y});
    currentLocation = tempLoc;
    if(currentLocation.x < 0 || currentLocation.y < 0) { return path; }
  }

  return path;
}

void mazeSolvingAlgorithm::computeShortestSearchPath()
{
  tileGrid.tiles[tileGrid.currentStart.y][tileGrid.currentStart.x].setDistance(0);

  Vector2 currentCoord = tileGrid.currentEnd;
  std::cout << "distance from start: " << tileGrid.tiles[tileGrid.currentStart.y][tileGrid.currentStart.x].getDistance() << std::endl;
  std::cout << "distance from end  : " << tileGrid.tiles[currentCoord.y][currentCoord.x].getDistance() << std::endl;

  // making sure that the algorithm was able to find the end
  if(!tileGrid.tiles[currentCoord.y][currentCoord.x].isSeen()) { return; }
  std::cout << "computation call" << std::endl;

    Vector2 neighborCoord;
  // backtracking from the end to the start
  while(tileGrid.tiles[currentCoord.y][currentCoord.x].getDistance() != 0)
  {
  //std::cout << "starting call" << std::endl;
    if(currentCoord.x < 0 || currentCoord.x >= tileGrid.cellCountX) { break; }
    if(currentCoord.y < 0 || currentCoord.y >= tileGrid.cellCountY) { break; }

    int lowestIndex(0);
    //Vector2 neighborCoord;
    //std::cout << " -----> (" << currentCoord.x << ", " << currentCoord.y << ") ==> goal " << "(" << tileGrid.currentEnd.x << ", " << tileGrid.currentEnd.y << "), start: (" << tileGrid.currentStart.x << ", " << tileGrid.currentStart.y << ")" << std::endl;
    for(int i=1;i<searchPattern.size();++i)
    {
      neighborCoord = Vector2
      {
        currentCoord.x + searchPattern[i].x,
        currentCoord.y + searchPattern[i].y
      };

      if(neighborCoord.x < 0 || neighborCoord.x >= tileGrid.cellCountX) { continue; }
      if(neighborCoord.y < 0 || neighborCoord.y >= tileGrid.cellCountY) { continue; }

      //std::cout << "     (" << neighborCoord.x << ", " << neighborCoord.y << ")" << std::endl;

      if
      (
        tileGrid.tiles[neighborCoord.y][neighborCoord.x].getDistance() != -1
        &&
        // neighbord cell
        tileGrid.tiles[neighborCoord.y][neighborCoord.x].getDistance() 
        < 
        // current lowest
        tileGrid.tiles[currentCoord.y][currentCoord.x].getDistance()
      )
      {
        lowestIndex = i;
      }
    }
    //std::cout << " <<<< " << std::endl;

    // the next neighbor on the shortest explored path
    neighborCoord = Vector2
    {
      currentCoord.x + searchPattern[lowestIndex].x,
      currentCoord.y + searchPattern[lowestIndex].y
    };

    shortestPath.push_back(Vector4{currentCoord.x,currentCoord.y,neighborCoord.x,neighborCoord.y});
    //std::cout << "distance " << tileGrid.tiles[currentCoord.y][currentCoord.x].getDistance();
    currentCoord = neighborCoord;
    //std::cout << " ---> " << tileGrid.tiles[currentCoord.y][currentCoord.x].getDistance() << std::endl;
  }
  std::cout << "computed shortest explored path from the end to start" << std::endl;
    shortestPath.push_back(Vector4{currentCoord.x,currentCoord.y,neighborCoord.x,neighborCoord.y});

  /*
      find the lowest distance neighbor at the given cell by going through the search patterns and then we go to that neighbor, we also add the current path to the list

  using the search pattern, we want to check for the shortest path
  check all the neighbors around this cell and see if we 
  shortestPath.push
  */

}

void mazeSolvingAlgorithm::computeSearchPath()
{
  //searchPath = vector<Vector4>();
  for(int i=0;i<endPlacements.size();++i)
  {
    //std::cout << "[" << i << "]" << std::endl;
    vector<Vector4> path = wanderedPathFromCell(endPlacements[i],tileGrid,false);
    for(int j=0;j<path.size();++j)
    {
      searchPath.push_back(path[j]);
    }
  }

  if(!foundShortestPath) { return; }

  std::cout << "computing from end\n";
  directPath   = wanderedPathFromCell(tileGrid.currentEnd,tileGrid,true);
  computeShortestSearchPath();
}


vector<Vector4> mazeSolvingAlgorithm::getSearchPath() { return searchPath; }


void mazeSolvingAlgorithm::loadMaze(const grid& originalMaze) 
{
  tileGrid = originalMaze;
  loadedMaze = true;
}

