#ifndef PANE_H
#define PANE_H

#include "mazeSolvingAlgorithm.h"
#include "menu.h"
#include "grid.h"

class pane
{
  int x;
  int y;
  int width;
  int height;
  bool menuOpen;
  Color backgroundColor;
  menu algorithmChooser;
  mazeSolvingAlgorithm* currentAlgorithm;
  //vector<mazeSolvingAlgorithm*> algorithmChoices;
  vector<string> algorithmChoices;
  float stepCount;
  
  void displayMenu();

  public:
  // ( x , y , width , height , buttons, color of buttons, text on buttons, fontSize)
  pane(int,int,int,int,vector<Rectangle>,vector<Color>,vector<string>,int); 

  // runs the step function on the selected algorithm
  void step();

  /*
   * checks if the user has clicked on any of the menu options
   * this includes sub menus like the change prioritization method for DFS
   */
  bool update(const Rectangle&, const bool&); 

  // displays the maze and the current menu options
  void display();
  void setBackgroundColor(const Color& newColor);
  void resizeGridToWindow();
  void setAlgorithm(const grid&);

  
};



#endif
