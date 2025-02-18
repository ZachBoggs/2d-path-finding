#include "../header/menu.h"
#include "raylib.h"

#include <string>
#include <vector>

#include<iostream>
using std::string;

using std::vector;
using std::cout;

menu::menu()
  : menuSize(-1)
{
}
menu::menu(int x, int y, vector<Rectangle> buttons, vector<Color> colors, vector<string> text, int nfontSize)
  : menuSize(buttons.size()),lastSelected(-1), touchingButton(-1), menuColors(colors), menuText(text), fontSize(nfontSize)
{
  
  for(int i=0;i<buttons.size();++i)
  {
    buttons[i].x += x;
    buttons[i].y += y;
  }
  menuButtons = buttons;

  for(int i=0;i<menuText.size();++i)
  {
    int leftOffset  = menuButtons[i].x + (menuButtons[i].width - MeasureText(menuText[i].c_str(),fontSize)) / 2;
    int rightOffset = menuButtons[i].y + ((menuButtons[i].height - fontSize) / 2);

    menuTextLocations.push_back(Vector2{leftOffset, rightOffset});
  }
}

void menu::display()
{
  //DrawRectangle(0,0,200,200,ORANGE);
  if(menuSize < 0)
  {
    std::cout << "calling display on non-initialized menu" << std::endl;
    return;
  }
  //cout << "display:\n";
  for(int i=0;i<menuSize;++i)
  {
    //cout << "(" << menuButtons[i].x << "," << menuButtons[i].y << ")\n";
    if(touchingButton == i)
    {
      DrawRectangleRec(menuButtons[i], menuColors[i % menuColors.size()]);
      DrawRectangleRec(menuButtons[i], Color{0,0,0,50});
    }else
    {
      DrawRectangleRec(menuButtons[i], menuColors[i % menuColors.size()]);
    }
    DrawText(menuText[i].c_str(),menuTextLocations[i].x, menuTextLocations[i].y,fontSize,WHITE);
  }
}

bool menu::checkCollisions(const Rectangle& mouseLocation, const bool& mouseButtonPressed)
{
  bool buttonContact(false);
  for(int i=0;i<menuSize;++i)
  {
    if(CheckCollisionRecs(menuButtons[i], mouseLocation))
    {
      touchingButton = i;
      buttonContact  = true;
      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      {
        lastSelected = i;
        return true;
      }
    }
  }
  if(!buttonContact)
  {
    touchingButton = -1;
  }
  return false;
}

int menu::getLastSelected()
{
  return lastSelected;
}

int menu::getTouchedButton()
{
  return touchingButton;
}

Rectangle menu::getButtonBox(const int& index)
{
  if(index < 0 || index >= menuButtons.size())
  {
    return menuButtons[0];
  }
  return menuButtons[index];
}




