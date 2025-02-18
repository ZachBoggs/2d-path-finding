#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <map>
#include <algorithm>
#include <random>
#include "raylib.h"
#include "grid.h"
#include "cell.h"
#include "menu.h"

using std::vector;
using std::string;

grid::grid
(
  const int    & xLoc   ,
  const int    & yLoc   ,
  const float  & gridW  ,
  const float  & gridH  ,
  const int    & cCountX,
  const int    & cCountY,
  const float  & tBarWid,
  const float  & cGap
) 
: 
  x           (xLoc   ),
  y           (yLoc   ),
  gridWidth   (gridW  ),
  gridHeight  (gridH  ),
  cellCountX  (cCountX), 
  cellCountY  (cCountY), 
  cellWidth   (((gridWidth - tBarWid) - (cGap * (cCountX-1)))/ cCountX),
  cellHeight  ((gridHeight - (cGap * (cCountY-1)))/ cCountY),
  cellGap     (cGap),
  tiles       (gridH,vector<cell>(gridW)),
  previewTileList(0),
  showToolBar (false),
  toolBarWidth(tBarWid),
  onWindow    (false),
  fillRadius  (5.1f),
  mouseIconSize(40.0f),
  currentMouseIcon(-1),
  drawFill(false),
  drawPreview(true),
  randomFillPercent(0.9f),
  currentCellType(0),
  currentStart{-1.0f,-1.0f},
  currentEnd{-1.0f,-1.0f}
{
  if(cellWidth < cellHeight) 
       { cellHeight = cellWidth;  }
  else { cellWidth  = cellHeight; }

  for(int i=0;i<cCountY;++i)
  {
    for(int j=0;j<cCountX;++j)
    {
      tiles[i][j] = cell((cellGap * i) + x + (i * cellWidth),(cellGap * j) + y + (j*cellWidth),cellWidth);
    }
  }

  mouseIcons = vector<Texture2D>();
  textureNames = {"pencil.png", "eraser.png"};

  for(const string& name : textureNames)
  {
    string assetPath = "../assets/";
    assetPath += name;

    Image loadedImage = LoadImage(assetPath.c_str());
    ImageResize(&loadedImage,mouseIconSize, mouseIconSize);

    mouseIcons.push_back(LoadTextureFromImage(loadedImage));
  }




  vector<Rectangle> buttonShapes;
  vector<Color>     buttonColors;
  vector<Color>     alternatingColors
  {
    Color{255,150,150,255},
    Color{255,100,100,255},
    Color{255,50 ,50 ,255},
    Color{235,90 ,90 ,255}
  };

  for(int i=0;i<textureNames.size();++i)
  {
    buttonShapes.push_back(Rectangle{0,(float)i*100,toolBarWidth,100});
    buttonColors.push_back(alternatingColors[i % 2]);
  }

  toolBarMenu = menu
  (
    x + gridWidth - toolBarWidth,
    y,
    buttonShapes,
    buttonColors,
    textureNames,
    12
  );

  vector<Rectangle> cellSelectionShapes =
  {
    Rectangle { 0.0f                     ,0.0f,toolBarWidth/3,50 },
    Rectangle { ((float)toolBarWidth)/3  ,0.0f,toolBarWidth/3,50 },
    Rectangle { (float)toolBarWidth/3 * 2,0.0f,toolBarWidth/3,50 }
  };
  vector<Color> cellSelectionColors = 
  {
    Color{120,120,120,255},
    Color{140,200,140,255},
    Color{255,0  ,0  ,255}
  };
  vector<string> cellSelectionNames = 
  {
    "wall",
    "start",
    "end"
  };

  drawCellSelector = menu
  (
    x + gridWidth - toolBarWidth,
    y + 200,
    cellSelectionShapes,
    cellSelectionColors,
    cellSelectionNames,
    12
  );


  modifierNames = {"fill","preview","+ size","- size","random","reset","fill % +","fill % -","left","right"};
  vector<Rectangle> modifierShapes;
  vector<Color>     modifierColors;
  for(;modifierNames.size() < 8;)
  {
    modifierNames.push_back("");
  }

  for(int i=0;i<modifierNames.size();++i)
  {
    modifierShapes.push_back(Rectangle{(float)(i%2) * 50,(float)(i/2)*50,toolBarWidth/2,50});
    modifierColors.push_back(alternatingColors[i%4]);
  }
  modifierNames.push_back("DONE");
  modifierShapes.push_back(Rectangle{0.0f,250.0f,toolBarWidth,50});
  modifierColors.push_back(Color{140,200,140,255});

  toolBarModifierMenu = menu
  (
    x + gridWidth - toolBarWidth,
    400,
    modifierShapes,
    modifierColors,
    modifierNames ,
    12
  );

  toolBarModifierMenu.menuColors[0] = RED;
  toolBarModifierMenu.menuColors[1] = GREEN;
  toolBarModifierMenu.menuColors[5] = ORANGE;
}

grid::grid(){}

void grid::draw(const Vector2& mouseLoc)
{
  for(int i=0;i<cellCountY;++i)
  {
    for(int j=0;j<cellCountX;++j)
    {
      tiles[i][j].draw();
    }
  }
  // displaying a preview if the user has this option toggled
  if(drawPreview)
  {
    displayDrawPreview();
  }

  if(showToolBar)
  {
    DrawRectangle(x + gridWidth - toolBarWidth,y,toolBarWidth,gridHeight - toolBarWidth,RED);

    if(onWindow)
    {
      switch(currentMouseIcon)
      {
        case 0:
          DrawTexture(mouseIcons.at(currentMouseIcon),mouseLoc.x, mouseLoc.y - mouseIconSize, Color{255,255,255,255});
          break;
        case 1:
          DrawTexture(mouseIcons.at(currentMouseIcon),mouseLoc.x, mouseLoc.y - mouseIconSize, Color{255,255,255,255});
          break;
        default:
          break;
      }


        //DrawRectangle(x + gridWidth - toolBarWidth,y,toolBarWidth,gridHeight - toolBarWidth,RED);
    }

    toolBarMenu.display();

    drawCellSelector.display();

    int selectedIndex = drawCellSelector.getLastSelected();
    if(selectedIndex != -1) { selectedIndex = 0; }

    Rectangle buttonLocation = drawCellSelector.getButtonBox(0);
    float notchSize(10.0f);

    toolBarModifierMenu.display();
    DrawRectangle(x + gridWidth + (selectedIndex * buttonLocation.width) + buttonLocation.x + buttonLocation.width/2 - notchSize/2  ,y + buttonLocation.y + buttonLocation.height + 10.0f,notchSize,notchSize,WHITE);
  }
}

void grid::resize(float& newX,float& newY,float& newWidth,float& newHeight)
{
  x          = newX;
  y          = newY;
  gridWidth  = newWidth;
  gridHeight = newHeight;

  if(showToolBar)
  {
    cellWidth  = (((gridWidth - toolBarWidth) - (cellGap * (cellCountX-1)))/ cellCountX);
  }else
  {
    cellWidth  = ((gridWidth - (cellGap * (cellCountX-1)))/ cellCountX);
  }
  cellHeight = ((gridHeight - (cellGap * (cellCountY-1)))/ cellCountY);

  if(cellWidth < cellHeight) 
       { cellHeight = cellWidth;  }
  else { cellWidth  = cellHeight; }

  std::cout << "\n";
  std::cout << "new width: " << gridWidth << ", new height: " << gridHeight << std::endl;
 

  //calculate out the remaining space of the grid form the top and bottom and center it
  //this applies specifically for pane count of 2 when the grids are on the top instead of in the center

  //float centerOffsetX = (gridWidth  - (cellCountX * cellWidth)) / 2.0f;
  //float centerOffsetY = (gridHeight - (cellCountY * cellWidth)) / 2.0f;

  for(int i=0;i<cellCountY;++i)
  {
    for(int j=0;j<cellCountX;++j)
    {
      //tiles[i][j].setCellBox(Rectangle{(cellGap * i) + x + (i * cellWidth) + centerOffsetX,(cellGap * j) + y + (j*cellWidth) + centerOffsetY,cellWidth,cellWidth});
      tiles[i][j].setCellBox(Rectangle{(cellGap * i) + x + (i * cellWidth),(cellGap * j) + y + (j*cellWidth),cellWidth,cellWidth});
    }
  }
}

//  fix constructor with texture, index or scale not being initialized

void grid::reset()
{
  tiles = vector<vector<cell>>(gridHeight,vector<cell>(gridWidth));
  currentStart = {-1.0f,-1.0f};
  currentEnd   = {-1.0f,-1.0f};

  for(int i=0;i<cellCountY;++i)
  {
    for(int j=0;j<cellCountX;++j)
    {
      tiles[i][j] = cell((cellGap * i) + x + (i * cellWidth),(cellGap * j) + y + (j*cellWidth),cellWidth);
    }
  }
}

void grid::randomize()
{
  reset();

  vector<Vector2> coordinateList;
  
  for(int i=0;i<cellCountY;++i)
  {
    for(int j=0;j<cellCountX;++j)
    {
      coordinateList.push_back(Vector2{(float)j,(float)i});
    }
  }
  srand(time(0));

  std::random_device rd;
  std::mt19937 g(rd());
  shuffle(coordinateList.begin(),coordinateList.end(),g);

  int xRand;
  int yRand;

  int fillCount = (randomFillPercent * ((cellCountX * cellCountY) - 2));
  for(int i=0;i<fillCount && coordinateList.size() > 2;++i)
  {
    tiles[coordinateList[0].y][coordinateList[0].x].setWallStatus(true);
    coordinateList.erase(coordinateList.begin());
  }

  currentStart = Vector2{coordinateList[0].x,coordinateList[0].y};
  currentEnd   = Vector2{coordinateList[1].x,coordinateList[1].y};
  tiles[coordinateList[0].y][coordinateList[0].x].toggleStart();
  tiles[coordinateList[1].y][coordinateList[1].x].toggleEnd();
}

  

int grid::updateCells(const Rectangle& mousePosRec,const Vector2& mouseLoc, const bool& mouseButtonDown)
{

  // if the mouse isn't in bounds, then we return
  if     (mousePosRec.x < x)                     { onWindow = false; }
  else if(mousePosRec.y < y)                     { onWindow = false; }
  else if(mousePosRec.x > (float)x + gridWidth ) { onWindow = false; }
  else if(mousePosRec.y > (float)y + gridHeight) { onWindow = false; }
  else
  {
    onWindow = true;
  }

  // if the user isn't touching the grid, then don't continue
  if(!onWindow) { return -1; }

  if(toolBarMenu.checkCollisions(mousePosRec,IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
  {
    currentMouseIcon = toolBarMenu.getLastSelected();

    std::cout << "tool pressed button: " << currentMouseIcon << std::endl;
  }

  if(drawCellSelector.checkCollisions(mousePosRec,IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
  {
    currentCellType = drawCellSelector.getLastSelected();
    /*
     * 0 - wall  cell
     * 1 - start cell
     * 2 - end   cell
     */
  }

  if(toolBarModifierMenu.checkCollisions(mousePosRec,IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
  {
    //std::cout << "modifier button pressed: " << toolBarModifierMenu.getLastSelected() << std::endl;

    switch(toolBarModifierMenu.getLastSelected())
    {
      case 0:
        drawFill = !drawFill;
        toolBarModifierMenu.menuColors[0] = (drawFill ? GREEN : RED);
        
        break;
      case 1:
        drawPreview = !drawPreview;
        toolBarModifierMenu.menuColors[1] = (drawPreview ? GREEN : RED);
        break;
      case 2:
        fillRadius += 0.5f;
        break;
      case 3:
        fillRadius -= 0.5f;
        if(fillRadius < 0.9f)
        {
          fillRadius = 0.9f;
        }
        break;
      case 4:
        randomize();

        break;
      case 5:
        reset();
        break;
      case 6:
        if(IsKeyDown(KEY_LEFT_SHIFT))
        {
          randomFillPercent += 0.1f;
        }else
        {
          randomFillPercent += 0.01f;
        }

        if(randomFillPercent > 100.0f) { randomFillPercent = 100.0f; }

        break;
      case 7:
        if(IsKeyDown(KEY_LEFT_SHIFT))
        {
          randomFillPercent -= 0.1f;
        }else
        {
          randomFillPercent -= 0.01f;
        }

        if(randomFillPercent < 0.0f) { randomFillPercent = 0.0f; }

        break;
      case 8:
        // going to the left grid
        return 1;
      case 9:
        // going to the right grid
        return 2;
      case 10:
        return 0;
        break;

      default:
        break;
    }
  }
  
  if(IsKeyDown(KEY_LEFT_SHIFT))
  {
    if(IsKeyDown(KEY_LEFT_BRACKET))
    {
    fillRadius -= 0.5f;
    }else if(IsKeyDown(KEY_RIGHT_BRACKET))
    {
      fillRadius += 0.5f;
    }

  }else
  {
    if(IsKeyPressed(KEY_LEFT_BRACKET))
    {
    fillRadius -= 0.5f;
    }else if(IsKeyPressed(KEY_RIGHT_BRACKET))
    {
      fillRadius += 0.5f;
    }
  }

  // drawing the preview if the option is toggled
  if(drawPreview) { makePreview(mouseToCellCoord(mouseLoc)); }

  // checking if we need to actually update anything
  if(!mouseButtonDown) { return -1; }

  // if we are curently drawing
  if(currentMouseIcon != -1)
  {
    Vector2 gridLocation = mouseToCellCoord(mouseLoc);

    if(currentMouseIcon == 0)
    {
      if(!((gridLocation.x < 0.0f || gridLocation.x >= cellCountX) || (gridLocation.y < 0.0f || gridLocation.y >= cellCountY)))
      {
        switch(currentCellType)
        {
          case 0:
            fillCircleGap(gridLocation.x,gridLocation.y, true);
            break;
          case 1:
            if(currentStart.x >= 0.0f && (currentEnd.x != gridLocation.x || currentEnd.y != gridLocation.y))
            {
              // resetting previous tile that was selected
              if(tiles[currentStart.y][currentStart.x].isStart())
              {
                tiles[currentStart.y][currentStart.x].toggleStart();
              }
            }
            if((gridLocation.x != currentEnd.x) || (gridLocation.y != currentEnd.y))
            {
              currentStart = gridLocation;
              tiles[currentStart.y][currentStart.x].toggleStart();
            }
            break;
          case 2:
            if(currentEnd.x >= 0.0f && (currentStart.x != gridLocation.x || currentStart.y != gridLocation.y))
            {
              // resetting previous tile that was selected
              if(tiles[currentEnd.y][currentEnd.x].isEnd()) 
              { 
                tiles[currentEnd.y][currentEnd.x].toggleEnd(); 
              }
            }
              //update our cases so it accounts for when the node is being drawn on a current node
              if((gridLocation.x != currentStart.x) || (gridLocation.y != currentStart.y))
              {

                currentEnd = gridLocation;
                tiles[currentEnd.y][currentEnd.x].toggleEnd();
              }
            break;
        }
      }
    }else if(currentMouseIcon == 1)
    {
      fillCircle(gridLocation.x,gridLocation.y, false,!drawFill);
    }
  }
  return -1;

}

void grid::toggleToolBar () 
{ 
  showToolBar = !showToolBar; 
  resize(x,y,gridWidth,gridHeight);
}
void grid::togglePreview()  { drawPreview = !drawPreview; }

void grid::fillCircleGap(const int& originX,const int& originY, const bool& status) 
{
  fillCircle(originX,originY,status,!drawFill);
}

void grid::fillCircle(const int& originX,const int& originY, const bool& status, const bool& ignoreWallStatus) 
{
  if(originX < 0 || originX >= cellCountX) { return; }
  if(originY < 0 || originY >= cellCountY) { return; }
  vector<Vector2> cellCoords;


  cellCoords.push_back(Vector2{(float)originX,(float)originY});
  std::map<int,std::map<int,bool>> isCellSet;

  while(cellCoords.size() != 0)
  {
    Vector2 currentCellCoord = cellCoords[0];
    cellCoords.erase(cellCoords.begin());

    if(ignoreWallStatus || tiles[currentCellCoord.y][currentCellCoord.x].isWall() != status)
    {
      
      tiles[currentCellCoord.y][currentCellCoord.x].setWallStatus(status);
      float currentCellDistance = sqrt(pow(currentCellCoord.x - originX,2) + pow(currentCellCoord.y - originY,2));

      vector<vector<int>> offsetPattern = {{0,1},{1,0},{0,-1},{-1,0}};
      for(const vector<int>& offset : offsetPattern)
      {
        int j=offset[0];
        int i=offset[1];

        int xLoc = currentCellCoord.x + j;
        int yLoc = currentCellCoord.y + i;

        // if this location is out of bounds, then we go to the next position
        if(xLoc < 0 || xLoc >= cellCountX) { continue; }
        if(yLoc < 0 || yLoc >= cellCountY) { continue; }

        float cellDistance = sqrt(pow(currentCellCoord.x + (i != 0 ?0:j) - originX,2) + pow(currentCellCoord.y + i - originY,2));
        if(cellDistance <= fillRadius && !isCellSet[yLoc][xLoc])
        {
          cellCoords.push_back(Vector2{(float)xLoc,(float)yLoc});
          isCellSet[yLoc][xLoc] = true;
        }
      }
    }

  }
}

Vector2 grid::mouseToCellCoord(const Vector2& mouseLoc)
{
  Vector2 cellCoords = Vector2{-1.0f,-1.0f};

  // if we aren't in bounds then return
  if     (mouseLoc.x < x)                     { return cellCoords; }
  else if(mouseLoc.y < y)                     { return cellCoords; }
  else if(mouseLoc.x > (float)x + gridWidth ) { return cellCoords; }
  else if(mouseLoc.y > (float)y + gridHeight) { return cellCoords; }

  if(cellGap == 0.0f)
  {
    float retX = (int)((mouseLoc.y - y) / cellWidth);
    float retY = (int)((mouseLoc.x - x) / cellWidth);
    //std::cout << "x: " << retX << ", y: " << retY << std::endl;
    return Vector2
    {
      retX,
      retY
    };
  }

  return cellCoords;
}

vector<Vector2> grid::makeCirclePreview(const Vector2& mouseLoc,const bool& ignoreWallStatus, const bool& status)
{
  vector<Vector2> affectedCells;
  if(mouseLoc.x < 0 || mouseLoc.x >= cellCountX) { return affectedCells; }
  if(mouseLoc.y < 0 || mouseLoc.y >= cellCountY) { return affectedCells; }


  Vector2 gridLocation = mouseLoc;

  vector<Vector2> cellCoords;
  
  // adding the starting location to the queue
  cellCoords   .push_back(gridLocation);
  affectedCells.push_back(gridLocation);

  std::map<int,std::map<int,bool>> isCellSet;
  

  while(cellCoords.size() > 0)
  {
    Vector2 currentCellCoord = cellCoords[0];
    cellCoords.erase(cellCoords.begin());

    if
    (
      (
        (
          isCellSet[currentCellCoord.y][currentCellCoord.x] == false
        )
        && 
        (
          (
            ignoreWallStatus
            &&
            (
              tiles[currentCellCoord.y][currentCellCoord.x].isWall() != status 
            )
          )
          ||
          (
            !ignoreWallStatus
          )
        )
      )
    )
    {
        
      //tiles[currentCellCoord.y][currentCellCoord.x].setWallStatus(status);
      isCellSet[currentCellCoord.y][currentCellCoord.x] = true;
      affectedCells.push_back(Vector2{currentCellCoord.x,currentCellCoord.y});
      //float currentCellDistance = sqrt(pow(currentCellCoord.x - gridLocation.x,2) + pow(currentCellCoord.y - gridLocation.y,2));

      vector<vector<int>> offsetPattern = {{0,1},{1,0},{0,-1},{-1,0}};
      for(const vector<int>& offset : offsetPattern)
      {
        int j=offset[0];
        int i=offset[1];

        int xLoc = currentCellCoord.x + j;
        int yLoc = currentCellCoord.y + i;

        // if the cell isn't in range
        if(xLoc < 0 || xLoc >= cellCountX) { continue; }
        if(yLoc < 0 || yLoc >= cellCountY) { continue; }

        // getting the distance from the current node to the center of the current circle
        float cellDistance = sqrt(pow(gridLocation.x - xLoc,2) + pow(gridLocation.y - yLoc,2));
        if((cellDistance <= fillRadius) && isCellSet[yLoc][xLoc] == false)
        {
          //std::cout << "xLoc: " << xLoc << ", yLoc: " << yLoc << std::endl;
          cellCoords.push_back   (Vector2{(float)xLoc,(float)yLoc});
        }
      }
    }
  }
  return affectedCells;
}

void grid::makePreview(const Vector2& mouseLoc)
{
  if(currentMouseIcon == 0)
  {
    if(mouseLoc.x >= 0.0f && mouseLoc.x < cellCountX && mouseLoc.y >= 0.0f < cellCountY)
    {
      // if we are drawing normal walls
      if(currentCellType == 0)
      {
        previewTileList = makeCirclePreview(mouseLoc, drawFill, true);
        //previewTileList = makeCirclePreview(mouseLoc, true, drawFill);
      }
    }
  }else if(currentMouseIcon == 1)
  {
    previewTileList = makeCirclePreview(mouseLoc, drawFill, false);
    //previewTileList = makeCirclePreview(mouseLoc, false, !drawFill);
  }

  //if(previewTileList.size() != 0) {std::cout << "size: " << previewTileList.size() << std::endl;}

  return;
}

void grid::displayDrawPreview()
{
  float diff = 200.0f / (float)previewTileList.size();
  for(int i=0;i<previewTileList.size();++i)
  {
    DrawRectangleRec(tiles[previewTileList[i].y][previewTileList[i].x].getCellBox(), Color{0,255,0,50});
    //DrawRectangleRec(tiles[previewTileList[i].y][previewTileList[i].x].getCellBox(), Color{255,0,0,150});
    //DrawRectangleRec(tiles[previewTileList[i].y][previewTileList[i].x].getCellBox(), Color{0,255,0,255 - (diff * i)});

  }
}

void grid::setRandomFillPercent(float newPercent)
{
  randomFillPercent = newPercent;
}


void grid::operator=(const grid& rhs)
{
  x = rhs.x;
  y = rhs.y;
  cellCountX = rhs.cellCountX;
  cellCountY = rhs.cellCountY;
  
  tiles = vector<vector<cell>>(rhs.cellCountY,vector<cell>(rhs.cellCountX));
  for(int i=0;i<cellCountY;++i)
  {
    for(int j=0;j<cellCountX;++j)
    {
      tiles[i][j] = rhs.tiles[i][j];
    }
  }

  gridWidth  = rhs.gridWidth;
  gridHeight = rhs.gridHeight;
  cellWidth  = rhs.cellWidth;
  cellHeight = rhs.cellHeight;
  cellGap    = rhs.cellGap;

  previewTileList     = {};
  showToolBar         = rhs.showToolBar;
  toolBarWidth        = rhs.toolBarWidth;
  mouseIcons          = rhs.mouseIcons;
  currentMouseIcon    = rhs.currentMouseIcon;
  mouseIconSize       = rhs.mouseIconSize;
  textureNames        = rhs.textureNames;
  modifierNames       = rhs.modifierNames;
  toolBarMenu         = rhs.toolBarMenu;
  drawCellSelector    = rhs.drawCellSelector;
  toolBarModifierMenu = rhs.toolBarModifierMenu;
  fillRadius          = rhs.fillRadius;
  drawFill            = rhs.drawFill;
  drawPreview         = rhs.drawPreview;
  randomFillPercent   = rhs.randomFillPercent;
  currentCellType     = rhs.currentCellType;
  currentStart        = rhs.currentStart;
  currentEnd          = rhs.currentEnd;

  std::cout << "copying over the start at x: " << currentStart.x << ", y: " << currentStart.y << std::endl;
  resize(x,y,gridWidth,gridHeight);
}

bool grid::readyForPathFinding()
{
  if
  (
    (
      currentStart.x >= 0.0f 
      && 
      currentStart.x <= cellCountX
    )
    &&
    (
      currentStart.y >= 0.0f 
      && 
      currentStart.y <= cellCountY
    )
    &&
    (
      currentEnd.x >= 0.0f 
      && 
      currentEnd.x <= cellCountX
    )
    &&
    (
      currentEnd.y >= 0.0f 
      && 
      currentEnd.y <= cellCountY
    )
  )
  {
    return true;
  }
  return false;
}



