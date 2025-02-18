#ifndef DFSUPDATED_H
#define DFSUPDATED_H

#include <string>
#include <vector>
#include "mazeSolvingAlgorithm.h"
#include "grid.h"

using std::string;
using std::vector;


class dfsUpdated : public mazeSolvingAlgorithm
{
  /*
  bool goUp();
  bool goLeft();
  bool goDown();
  bool goRight();
  */

  //typedef bool (dfsUpdated::*directionFunc)();
  //vector<directionFunc> orderHolder;
  vector<int> directionOrder;

  //vector<Vector2> searchPattern;
  Vector2 currentLocation;
  //using directionFunc = void (dfsUpdated::*)();
  //vector<void (dfsUpdated::*)()> orderHolder;
  //vector<directionFunc> orderHolder;
  
  public:
    dfsUpdated(const grid&);
    ~dfsUpdated() = default;
  // runs the defined step function for each class
  void step();

  // checks if the class has a custom defined order in which directions are checked
  bool hasRule();
};



#endif
