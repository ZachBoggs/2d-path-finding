#include "raylib.h"
#include "cell.h"
#include <iostream>

cell::cell()
  : visited(false),seen(false),wall(false), cellBox(Rectangle{0,0,100,100}),setWallColor(false),startCell(false),endCell(false),foundingCell({-1.0f,-1.0f}),inPath(false),distance(-1)
{
}

cell::cell(const float& x, const float& y, const float& width)
  : visited(false),seen(false),wall(false), cellBox(Rectangle{x,y,width,width}),setWallColor(false),startCell(false),endCell(false),foundingCell({-1.0f,-1.0f}),inPath(false),distance(-1)
{
}

cell::cell(const float& x, const float& y, const float& width, const Color& wColor)
  : visited(false),seen(false),wall(false), cellBox(Rectangle{x,y,width,width}),setWallColor(true),wallColor(wColor),startCell(false),endCell(false),foundingCell({-1.0f,-1.0f}),inPath(false),distance(-1)
{
}

void cell::draw()
{
  Color cellColor;
  if     (startCell){ cellColor = GREEN; }
  else if(endCell)  { cellColor = RED; }
  else if(visited)  
  { 
    DrawRectangleRec(cellBox,GRAY);
    cellColor = Color{0,0,255,50}; 
  }
  else if(seen)     
  { 
    DrawRectangleRec(cellBox,GRAY);
    //cellColor = YELLOW;  
    cellColor = Color{255,255,155,55}; 
  }
  else if(wall)     { cellColor = (setWallColor ? wallColor : BLACK); }
  else              { cellColor = GRAY; }

  //DrawRectangle(x,y,width,width,cellColor);
  DrawRectangleRec(cellBox,cellColor);
}

void cell::resetTotal()
{
  visited = false;
  seen    = false;
  wall    = false;
}

void cell::resetSpecs()
{
  visited = false;
  seen    = false;
}

void cell::setWallStatus(const bool& status) { wall = status; }
void cell::toggleStart() { startCell = !startCell; }
void cell::toggleEnd()   { endCell   = !endCell; }
void cell::visit()    { visited   = true; }
void cell::see(const Vector2& newLocation)      
{ 
  foundingCell = newLocation;
  seen      = true; 
}

bool cell::isWall()    { return wall;     }
bool cell::isVisited() { return visited;  }
bool cell::isSeen()    { return seen;     }
bool cell::isStart()   { return startCell;}
bool cell::isEnd()     { return endCell;  }
const Rectangle& cell::getCellBox() { return cellBox; }
void cell::setCellBox(const Rectangle& inputRec) 
{ 
  cellBox.x      = inputRec.x;
  cellBox.y      = inputRec.y;
  cellBox.width  = inputRec.width;
  cellBox.height = inputRec.height;
}


Vector2 cell::getFoundingCell() 
{ 
  if(foundingCell.x < 0 || foundingCell.y < 0)
  {
    return Vector2{0.0f,0.0f};
  }
  //this variable is uninitialized somewhere
  
  return foundingCell; 
}
void cell::setFoundingCell(Vector2 newLocation) 
{ 
  foundingCell = newLocation; 
}

void cell::setDistance(int newDistance) { distance = newDistance; }
int  cell::getDistance()                { return     distance;    }

void cell::setUsedInPath() { inPath = true; }
bool cell::usedInPath()    { return inPath; }

void cell::operator=(const cell& rhs)
{
  startCell    = rhs.startCell;
  endCell      = rhs.endCell;
  visited      = rhs.visited;
  seen         = rhs.seen;
  wall         = rhs.wall;
  inPath       = rhs.inPath;
  cellBox      = rhs.cellBox;
  setWallColor = rhs.setWallColor;
  wallColor    = rhs.wallColor;
  foundingCell = rhs.foundingCell;
}




