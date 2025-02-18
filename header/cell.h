#ifndef CELL_H
#define CELL_H

#include "raylib.h"
#include <vector>

class cell
{
  bool startCell;
  bool endCell;
  bool visited;
  bool seen;
  bool wall;
  bool inPath;
  Rectangle cellBox;
  bool setWallColor;
  Color wallColor;
  Vector2 foundingCell; // the cell that found this one
  int distance;

  public:
  cell();
  cell(const float& x, const float& y, const float& width);
  cell(const float& x, const float& y, const float& width, const Color&);
  void draw();

  void resetTotal();
  void resetSpecs();

  void setWallStatus(const bool&);
  void toggleStart();
  void toggleEnd();
  void visit();
  void see(const Vector2&);

  bool isWall();
  bool isVisited();
  bool isSeen();
  bool isStart();
  bool isEnd();

  void setUsedInPath();
  bool usedInPath();

  Vector2 getFoundingCell();
  void setFoundingCell(Vector2);

  void setDistance(int);
  int  getDistance();

  const Rectangle& getCellBox();
  void setCellBox(const Rectangle&);
  void operator=(const cell&);
};

#endif
