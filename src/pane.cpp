#include "raylib.h"
#include "pane.h"
#include "bfs.h"
#include "dfs.h"
#include "dfsUpdated.h"
#include "AStar.h"
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::to_string;
using std::cout;

//pane::pane(int nx, int ny, int nWidth, int nHeight)
pane::pane(int nx, int ny, int nWidth, int nHeight,vector<Rectangle> buttons, vector<Color> colors, vector<string> text, int fontSize)
  : x(nx), y(ny), width(nWidth), height(nHeight), menuOpen(true),currentAlgorithm(nullptr), algorithmChooser(nx,ny,buttons, colors, text, fontSize), backgroundColor(RED),algorithmChoices{"bfs","dfs","dfs updated","Astar"},stepCount(1.0f)
{
}

void pane::step()
{
  // making sure that the user has an algorithm choses for the current panel
  if(currentAlgorithm == nullptr) { return; }

  for(int i=0;i<(int)stepCount;++i)
  {
    currentAlgorithm->step();
  }
  stepCount += 0.2f;
}

bool pane::update(const Rectangle& mouseLoc, const bool& lMouseButtonPressed)
{
  if(menuOpen)
  {
    if(algorithmChooser.checkCollisions(mouseLoc,lMouseButtonPressed))
    {
      menuOpen = false;
      //currentAlgorithm = algorithmChoices[algorithmChooser.getLastSelected()];
      return true;
      //cout << "user pressed button [" << algorithmChooser.getLastSelected() << "]\n";
    }
  }
  return false;
}

void pane::displayMenu()
{
  algorithmChooser.display();
}

void pane::display()
{
  DrawRectangle(x, y, width, height, backgroundColor);
  if(currentAlgorithm != nullptr)
  {
    currentAlgorithm->display();
  }

  if(menuOpen)
  {
    displayMenu();
    return;
  }

  string selectionNumber = "[";
  selectionNumber += to_string(algorithmChooser.getLastSelected());
  selectionNumber += "]: "; 
  //selectionNumber += algorithmChoices[algorithmChooser.getLastSelected()]->algorithmName;
  selectionNumber += algorithmChoices[algorithmChooser.getLastSelected()];

  DrawText(selectionNumber.c_str(),x,y,40,WHITE);
}

void pane::setAlgorithm(const grid& inputGrid)
{

  switch(algorithmChooser.getLastSelected())
  {
    // no menu button selected
    case -1:
      std::cerr << "trying to set algorithm when no algorithm has been set" << std::endl;
      break;

    // bfs
    case 0:
      currentAlgorithm = new bfs(inputGrid);
      break;

    // dfs
    case 1:
      currentAlgorithm = new dfs(inputGrid);
      break;

    // dfs
    case 2:
      currentAlgorithm = new dfsUpdated(inputGrid);
      break;

    // Astar
    case 3:
      currentAlgorithm = new AStar(inputGrid);
      break;

    default:
      std::cerr << "trying to set to an algorithm that doesn't exist in the panes switch case ( invaild entry )" << std::endl;
      std::cerr << "setting algorithm to bfs by default" << std::endl;
      currentAlgorithm = new bfs(inputGrid);
      break;
  }

  if(currentAlgorithm == nullptr) { return; }
  
  //currentAlgorithm->loadMaze(inputGrid);
  resizeGridToWindow();
  
}

void pane::setBackgroundColor(const Color& newColor)
{
  backgroundColor = newColor;
}

void pane::resizeGridToWindow()
{
  currentAlgorithm->resizeToWindow(x,y,width,height);
}



