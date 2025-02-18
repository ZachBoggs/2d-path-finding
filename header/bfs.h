#ifndef BFS_H
#define BFS_H

#include <string>
#include <vector>
#include "mazeSolvingAlgorithm.h"
#include "grid.h"

using std::string;
using std::vector;


class bfs : public mazeSolvingAlgorithm
{
  vector<Vector2> cellStack;
  //vector<Vector2> searchPattern;
  
  public:
    bfs(const grid&);
    ~bfs() = default;
  // runs the defined step function for each class
  void step();
  bool hasRule();
};



#endif
