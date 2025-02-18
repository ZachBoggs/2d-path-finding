#ifndef DFS_H
#define DFS_H

#include <string>
#include <vector>
#include "mazeSolvingAlgorithm.h"
#include "grid.h"

using std::string;
using std::vector;


class dfs : public mazeSolvingAlgorithm
{
  /*
  bool goUp();
  bool goLeft();
  bool goDown();
  bool goRight();
  */

  //typedef bool (dfs::*directionFunc)();
  //vector<directionFunc> orderHolder;
  vector<int> directionOrder;

  //vector<Vector2> searchPattern;
  Vector2 currentLocation;
  //using directionFunc = void (dfs::*)();
  //vector<void (dfs::*)()> orderHolder;
  //vector<directionFunc> orderHolder;
  
  public:
    dfs(const grid&);
    ~dfs() = default;
  // runs the defined step function for each class
  void step();

  // checks if the class has a custom defined order in which directions are checked
  bool hasRule();
};



#endif
