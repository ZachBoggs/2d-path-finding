#ifndef ASTAR_H
#define ASTAR_H

#include <string>
#include <vector>
#include "mazeSolvingAlgorithm.h"
#include "grid.h"

using std::string;
using std::vector;


class AStar : public mazeSolvingAlgorithm
{
  vector<Vector4> cellStack;
  
  public:
    AStar(const grid&);
    ~AStar() = default;

  // runs the defined step function for each class
  void step();
  bool hasRule();

};

#endif
