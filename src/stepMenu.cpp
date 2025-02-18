#include "backgroundSystem.h"
#include "cameraManagement.h"
#include "stepMenu.h"
#include "menu.h"
#include "pane.h"
#include "raylib.h"
#include <iostream>

using std::string;
using std::to_string;

void runAlgorithm(Camera3D& camera,int& rotationFrameCount,float& sinWaveValue,bool& backgroundOnlyMode,vector<pane>& paneHolder)
{
  bool realtimeRunning(false);
  vector<Rectangle> menuButtons;

  for(int i=0;i<5;++i)
  {
    menuButtons.push_back(Rectangle{0,(float)i * 40,100,40});
  }

  vector<Color> menuColors = { RED, BLUE, YELLOW, GREEN, PINK };
  vector<string> menuText =
  {
    "background",
    "step",
    "auto-run",
    "none",
    "none"
  };

  
  menu optionMenu(400,0,menuButtons,menuColors,menuText,20);




  Vector2   mousePos;
  Rectangle mousePosRec;
  bool mouseButtonPressed;

  bool showBackground(true);
  bool autoRun = false;

  while(!WindowShouldClose())
  {
    mousePos           = GetMousePosition();
    mousePosRec        = Rectangle{mousePos.x,mousePos.y,0,0};
    mouseButtonPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // triggers if a button is pressed on the menu
    if(optionMenu.checkCollisions(mousePosRec,mouseButtonPressed) )
    {
      switch(optionMenu.getLastSelected())
      {
        case 0:
          std::cout << "toggle background\n";
          showBackground = !showBackground;
          break;

        case 1:

          for(int i=0;i<paneHolder.size();++i)
          {
            paneHolder[i].step();
          }
          break;

        case 2:
          autoRun = !autoRun;

          break;

        default:
          break;
      }


      //std::cout << "stepping forward" << std::endl;
      //std::cout << "clicked button: " << to_string(optionMenu.getLastSelected()) << std::endl;
    }

    if(IsKeyDown(KEY_SPACE) || autoRun)
    {
      for(int i=0;i<paneHolder.size();++i)
      {
        paneHolder[i].step();
      }
    }
    //updateCameraRotation(camera,0.01f);
    if(showBackground)
    {
      updateCameraRotation(camera,0.001f);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    if(showBackground)
    {
      updateBackground(camera,rotationFrameCount,sinWaveValue,backgroundOnlyMode);
    }

    
    for(int i=0;i<paneHolder.size();++i)
    {
      paneHolder[i].display();
    }
    
    optionMenu.display();

    EndDrawing();
  }
}
