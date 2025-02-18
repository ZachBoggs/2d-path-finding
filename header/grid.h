#ifndef GRID_H
#define GRID_H

#include <vector>
#include "cell.h"
#include "menu.h"
//#include "mazeSolvingAlgorithm.h"

using std::vector;

class grid
{
  float x;
  float y;
  float gridWidth;
  float gridHeight;
  float cellGap;
  //vector<vector<cell>> previewTiles;
  vector<Vector2> previewTileList;

  bool showToolBar;
  float toolBarWidth;
  vector<Texture2D> mouseIcons;
  bool onWindow;
  int currentMouseIcon;
  float mouseIconSize;

  vector<string> textureNames;
  vector<string> modifierNames;
  menu toolBarMenu;
  menu drawCellSelector;
  menu toolBarModifierMenu;
  float fillRadius;
  bool drawFill;
  bool drawPreview;
  float randomFillPercent;
  int currentCellType;

  vector<Vector2> makeCirclePreview(const Vector2&, const bool&, const bool&);

  public:

  grid();

  grid
  (
    const int   & xLoc   , // x location of the grid
    const int   & yLoc   , // y location of the grid
    const float & gridW  , // width of the grid itself
    const float & gridH  , // height of the grid itself
    const int   & cCountX, // cell count in the x axis
    const int   & cCountY, // cell count in the y axis
    const float & tBarWid, // gap in between the cells
    const float & cGap     // gap in between the cells
  );

  void resize(float&,float&,float&,float&);
  void draw(const Vector2&);
  void reset();
  void randomize();
  int  updateCells(const Rectangle&,const Vector2&, const bool&);
  void toggleToolBar();
  void togglePreview();

  void fillCircleGap(const int&,const int&, const bool&); // fills a circle around where the mouse is at until it hits walls
  void fillCircle(const int&,const int&, const bool&,const bool&); // fills a circle around where the mouse is at regardless of walls

  Vector2 mouseToCellCoord(const Vector2&);
  void makePreview(const Vector2&);
  void displayDrawPreview();

  void setRandomFillPercent(float);
  void operator=(const grid&);
  bool readyForPathFinding();

  public:
  vector<vector<cell>> tiles;
  Vector2 currentStart;
  Vector2 currentEnd;
  int cellCountX;
  int cellCountY;
  float cellWidth;
  float cellHeight;
};

#endif
