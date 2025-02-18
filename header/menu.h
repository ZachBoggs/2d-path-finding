#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

class menu
{
  int menuSize;
  int lastSelected;
  int touchingButton;
  int fontSize;

  vector<Rectangle> menuButtons;
  vector<string>    menuText;
  vector<Vector2>   menuTextLocations;

  public:
  vector<Color>     menuColors;
  menu();
  /*
   * x coordinate
   * y coordinate
   * vector<rectangle> menu options
   * vector<color> button colors
   * vector<string> button text
   * int font size
   */
  menu(int,int,vector<Rectangle>, vector<Color>, vector<string>, int);
  void display();

  // returns true if the users mouse is clicked on any buttons
  bool checkCollisions(const Rectangle&, const bool&); 
  int  getTouchedButton();
  int  getLastSelected();
  Rectangle getButtonBox(const int&);
};

#endif

