#ifndef MAZESOLVINGALGORITHM_H
#define MAZESOLVINGALGORITHM_H

#include <string>
#include <vector>
#include "raylib.h"
#include "grid.h"

using std::string;
using std::vector;

class mazeSolvingAlgorithm
{
  private:
  vector<Vector4> wanderedPathFromCell(Vector2,grid&,const bool&);

  protected:
  vector<Vector2> searchPattern;
  vector<Vector2> endPlacements; // holds all of the locations that an algorithm had to backtrack from
  vector<Vector4> searchPath;
  vector<Vector4> directPath;
  vector<Vector4> shortestPath;
  //vector<Vector4> shortestPath;
  void computeSearchPath();
  void computeShortestSearchPath();
  grid tileGrid;
  bool loadedMaze;
  bool foundShortestPath;
  
  public:
  vector<Vector4> getSearchPath();
  mazeSolvingAlgorithm(string,vector<Vector2>);

  // runs the defined step function for each class
  virtual void step()    = 0;

  // checks if the class has a custom defined order in which directions are checked
  virtual bool hasRule() = 0; 
  void loadMaze(const grid&);
  void display();
  void drawSearchPath(Color);
  void resizeToWindow(float,float,float,float);
  
  string algorithmName;
};



#endif
