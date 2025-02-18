#ifndef SHORTCUTINFOMENU_H
#define SHORTCUTINFOMENU_H

#include "raylib.h"

using std::string;

class shortcutInfoMenu
{
  Color backgroundColor;
  Rectangle openIcon;
  int fontSize;
  int shortcutInfoWidth;
  int shortcutInfoHeight;
  bool infoOpen;

  vector<string> shortcutKey;
  vector<string> shortcutInfo;

  shortcutInfoMenu(vector<string>,vector<string>, Vector2,int);
  void toggleMenu();
  void updateCollisions(const Rectangle&,bool);
  void display();
}

#endif
