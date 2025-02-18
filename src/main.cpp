#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"
#include "rlgl.h"
#include "pane.h"
#include "menu.h"
#include "mazeSolvingAlgorithm.h"
//#include "dfs.h"
//#include "bfs.h"
#include "cell.h"
#include "grid.h"
#include "stepMenu.h"
#include "backgroundSystem.h"
#include "cameraManagement.h"
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

using std::vector;
using std::string;
using std::cout;
using std::to_string;


Camera3D camera = {0};
bool backgroundOnlyMode = false;
int rotationFrameCount(0);
float sinWaveValue(0);


int getPaneCount(const int& screenWidth, const int& screenHeight, const int& rightEdgeLimit)
{

  vector<Rectangle> menuRecs;
  for(int i=0;i<3;++i)
  {
    menuRecs.push_back(Rectangle{0,float(((screenHeight - 400) / 3) * i), (float)(screenWidth - rightEdgeLimit),float(screenHeight - 400) / 3});
  }
  //vector<Color>     menuColors{BLUE,RED,GREEN};
  vector<Color>     menuColors
  {
    Color{255,150,150,255},
    Color{255,100,100,255},
    Color{255,50 ,50 ,255},
    Color{235,90 ,90 ,255}
  };
  vector<string>    menuText{"One", "Two", "Four"};
  vector<int>       returnResults{1,2,4};

  //menu paneCountMenu(400 + screenWidth/2 - 150,250,menuRecs, menuColors, menuText,55);
  //menu paneCountMenu(400 + screenWidth/2 - 150,250,menuRecs, menuColors, menuText,55);
  menu paneCountMenu(rightEdgeLimit, ((screenHeight - 185) - (screenHeight - 400)) / 2 + 175,menuRecs, menuColors, menuText,55);

  vector<Rectangle> panePreviews[3];
	panePreviews[0] = vector<Rectangle>
	{
		Rectangle{0               ,0             ,float(rightEdgeLimit  ),float(screenHeight  )}
	};
	panePreviews[1] = vector<Rectangle>
	{
		Rectangle{0               ,0             ,float(rightEdgeLimit/2),float(screenHeight  )},
		Rectangle{float(rightEdgeLimit/2),0      ,float(rightEdgeLimit/2),float(screenHeight  )}
	};
	panePreviews[2] = vector<Rectangle>
	{
		Rectangle{0               ,0             ,float(rightEdgeLimit/2),float(screenHeight/2)},
		Rectangle{float(rightEdgeLimit/2),0             ,float(rightEdgeLimit/2),float(screenHeight/2)},
		Rectangle{0               ,float(screenHeight/2),float(rightEdgeLimit/2),float(screenHeight/2)},
		Rectangle{float(rightEdgeLimit/2),float(screenHeight/2),float(rightEdgeLimit/2),float(screenHeight/2)}
	};

	//vector<Color> panePattern{RED,BLUE,GREEN,YELLOW};
	vector<Color> panePattern
  {
    Color{255,150,150,255},
    Color{255,100,100,255},
    Color{255,50 ,50 ,255},
    Color{235,90 ,90 ,255}
    /*
    RED,
    BLUE,
    GREEN,
    YELLOW
    */
  };

  Vector2   mousePos;
  Rectangle mousePosRec;
  bool      mouseButtonPressed;
  int currentPreview(0);


  while (!WindowShouldClose()) 
  {
    updateCameraRotation(camera,0.001f);
    BeginDrawing();
    ClearBackground(BLACK);
    updateBackground(camera,rotationFrameCount,sinWaveValue,backgroundOnlyMode);

		if(backgroundOnlyMode)
		{
			EndDrawing();
			continue;
		}

    mousePos           = GetMousePosition();
    mousePosRec        = Rectangle{mousePos.x,mousePos.y,0,0};
    mouseButtonPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if(paneCountMenu.checkCollisions(mousePosRec, mouseButtonPressed))
    {
      cout << "user selected " << paneCountMenu.getLastSelected() + 1 << " panes\n";
      return returnResults[paneCountMenu.getLastSelected()];
    }

    for(int i=0;i<panePreviews[currentPreview].size();++i)
    {
      //DrawRectangleRec(panePreviews[currentPreview][i],( i % 3 == 0 ? RED : BLUE));
      DrawRectangleRec(panePreviews[currentPreview][i],Color{panePattern[i].r,panePattern[i].g,panePattern[i].b,200});
    }
    // vertical bar to divide out the page
    //DrawRectangle((screenWidth / 2) - 5,0,10,screenHeight,Color{0,0,0,50});

		DrawRectangle(0,85,screenWidth,100,BLACK);
		DrawRectangle(0,75,screenWidth,100,WHITE);
    DrawText("Pane Preview:",(screenWidth / 4) - MeasureText("Pane Preview:",50) /2 ,100,50,BLACK);
    DrawText
    (
     "Select Pane Amount:",
     (
       screenWidth / 2 + 
       screenWidth / 4 - 
       (
        MeasureText("Select Pane Amount:",50) / 2
       )
     ),
     100,
     50,
     BLACK
    );

    paneCountMenu.display();

    if(paneCountMenu.getTouchedButton() != -1)
    {
      // draw preview of options
      //cout << "touching button " << paneCountMenu.getTouchedButton() << "\n";
      currentPreview = paneCountMenu.getTouchedButton();
    }


    EndDrawing();
  }
	return 0;
}




void paneHandler(const int& screenWidth, const int& screenHeight, const int& rightEdgeLimit, const grid& inputGrid)
{
  Vector2   mousePos;
  Rectangle mousePosRec;
  bool      mouseButtonPressed;

  int userSelection = getPaneCount(screenWidth, screenHeight,rightEdgeLimit);
  string tempPrompt = "you selected " + to_string(userSelection) + " panes";

  // setting our pane rectangles
  vector<Rectangle> paneButtons;
  if(userSelection == 1)
  {
    paneButtons = vector<Rectangle>
    {
      Rectangle{0                      ,0                    ,float(rightEdgeLimit)  ,float(screenHeight)  }
    };
  }else if(userSelection == 2)
  {
    paneButtons = vector<Rectangle>
    {
      Rectangle{0                      ,0                    ,float(rightEdgeLimit/2),float(screenHeight  )},
      Rectangle{float(rightEdgeLimit/2),0                    ,float(rightEdgeLimit/2),float(screenHeight  )}
    };
  }else // 4 panes
  {
    paneButtons = vector<Rectangle>
    {
      Rectangle{0                      ,0                    ,float(rightEdgeLimit/2),float(screenHeight/2)},
      Rectangle{float(rightEdgeLimit/2),0                    ,float(rightEdgeLimit/2),float(screenHeight/2)},
      Rectangle{0                      ,float(screenHeight/2),float(rightEdgeLimit/2),float(screenHeight/2)},
      Rectangle{float(rightEdgeLimit/2),float(screenHeight/2),float(rightEdgeLimit/2),float(screenHeight/2)}
    };
  }

  vector<Color>     algorithmColors
  {
    Color{255,150,150,255},
    Color{255,100,100,255},
    Color{255,50 ,50 ,255},
    Color{135,90 ,90 ,255},
  };

  vector<string>    algorithmText =
  {
    "bfs",
    "dfs",
    "dfs updated",
    "Astar"
  };


  vector<Rectangle> algorithmButtons;
  // adding all the algorithms to the menus
  for(int i=0;i<algorithmText.size();++i)
  {
    algorithmButtons.push_back
    (
      Rectangle
      {
        paneButtons[0].width / 5,
        (paneButtons[0].height / algorithmText.size()) * i,
        3 * (paneButtons[0].width / 5),
        paneButtons[0].height / algorithmText.size()
      }
    );

    cout << "adding " << algorithmText[i] << " to text list\n";
  }

  vector<pane> paneHolder;


  for(int i=0;i<paneButtons.size();++i)
  {
    paneHolder.push_back(pane(paneButtons[i].x,paneButtons[i].y,paneButtons[i].width,paneButtons[i].height, algorithmButtons, algorithmColors, algorithmText,50));
    //paneHolder[i].setBackgroundColor(algorithmColors[i]);
    paneHolder[i].setBackgroundColor
    (
      Color { algorithmColors[i].r, algorithmColors[i].g, algorithmColors[i].b, 200 }
    );
  }

  int algorithmCount(0);
  bool doneLoadingAlgorithms(false);

  while (!WindowShouldClose()) 
  {

    // resetting when the player presses r
    if(IsKeyPressed(KEY_R)) { break; }


    updateCameraRotation(camera,0.001f);
    BeginDrawing();

    ClearBackground(BLACK);
    updateBackground(camera,rotationFrameCount,sinWaveValue,backgroundOnlyMode);
		if(backgroundOnlyMode)
		{
			EndDrawing();
			continue; 
		}

    mousePos           = GetMousePosition();
    mousePosRec        = Rectangle{mousePos.x,mousePos.y,0,0};
    mouseButtonPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // updating all our menus in case the mouse is hovering 
    // over a button or has clicked the mouse button
    for(int i=0;i<paneHolder.size();++i)
    {
      if(paneHolder[i].update(mousePosRec,mouseButtonPressed))
      {
        algorithmCount++;
        paneHolder[i].setAlgorithm(inputGrid);

        // once the amount of panels that have a 
        // selected algorithm reaches
        // the size of the paneHolder, then we 
        // start the simulation
        if(algorithmCount == paneHolder.size())
        {
          doneLoadingAlgorithms = true;
        }
      }
    }

		DrawRectangle(rightEdgeLimit,0,screenWidth-rightEdgeLimit,screenHeight,Color{255,255,255,200});
    //DrawText(tempPrompt.c_str(),rightEdgeLimit + (((screenWidth - rightEdgeLimit) - MeasureText(tempPrompt.c_str(),30)) / 2),20,30,BLACK);

    for(int i=0;i<paneHolder.size();++i)
    {
      paneHolder[i].display();
    }

    EndDrawing();
    if(doneLoadingAlgorithms) { break; }
  }


  if(doneLoadingAlgorithms) 
  { 
    
    for(int i=0;i<paneHolder.size();++i)
    {
      paneHolder[i].resizeGridToWindow();
    }

    runAlgorithm(camera,rotationFrameCount,sinWaveValue,backgroundOnlyMode,paneHolder);

    /*
    set algorithm for each panel
    load in the grid / maze on each panel
    from there we can run the algorithm?

    have it so it resizes to othe specific width where the tool bar changes the given width
    if the tool bar is enabled then the panel has the toolbar on the right, otherwise
    the panel itself ignores the width of the tool bar and just draws
    the grid

    there is a small bug where when a menu option is chosen for the algorithm that will be ran, the window displays the grid for a slight glimpse, and in the instance where the panel count is 2 or more, the grid is displayed over the other buttons


    after we add over the algorithms, we also want to disable the tool bar from being displayed


    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    find where we set the algorithm and update it so then it creates a copy of the current algorithm after selecting an algorithm
      push back the text of each algorithm instead of the algorithm itself?

      in this if statement we will then want to load each algorithm itself maybe using a switch case statement based off of what button was selected


      

      */

      // start running the algorithms on each pane
      // if we are in manual mode, wait until the step button is pressed
      // have it automatically run each algorithm and be able to just step through the maze in real time being able to go back and forward
      // push a vector of size 5 of the cells that would represent a cross + of the nodes that are affected at a turn?
      
  }

}


int main() 
{
  const int screenWidth  = 1920;
  const int screenHeight = 1080;
  const int rightEdgeLimit = 1500;

	//SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  
  InitWindow(screenWidth, screenHeight, "411 project");
  SetTargetFPS(60);
  cameraSetUpFunc(camera);
  float gridDim (1000.0f);
  //grid referenceGrid((screenWidth - gridDim) / 2,(screenHeight - gridDim) / 2,gridDim,gridDim,300,300,100.0f,0.0f);
  grid referenceGrid((screenWidth - gridDim) / 2,(screenHeight - gridDim) / 2,gridDim,gridDim,100,100,100.0f,0.0f);
  //grid referenceGrid((screenWidth - gridDim) / 2,(screenHeight - gridDim) / 2,gridDim,gridDim,20,20,100.0f,0.0f);
  //grid referenceGrid((screenWidth - gridDim) / 2,(screenHeight - gridDim) / 2,gridDim,gridDim,20,10,100.0f,0.0f);
  grid tileGrid = referenceGrid;//(0,0,800.0f,800.0f,100,100,100.0f,0.0f);
  tileGrid.toggleToolBar();
  referenceGrid.toggleToolBar();

  vector<grid> gridStack;
  gridStack.push_back(tileGrid);
  //tileGrid.togglePreview();
  tileGrid.setRandomFillPercent(0.1f);
  referenceGrid.setRandomFillPercent(0.1f);
  int currentGridIndex(0);

  while(!WindowShouldClose())
  {
    if(IsKeyPressed(KEY_R)) { tileGrid.reset(); }
    Vector2 mousePos      = GetMousePosition();
    Rectangle mousePosRec = Rectangle{mousePos.x,mousePos.y,0,0};
    bool mouseButtonDown  = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    int tileGridResult = tileGrid.updateCells(mousePosRec,mousePos,mouseButtonDown);
    // selected the current grid only if a start and end cell exist
    if(tileGridResult == 0 && tileGrid.readyForPathFinding())
    {
      tileGrid.toggleToolBar();
		  paneHandler(screenWidth,screenHeight,rightEdgeLimit,tileGrid);
      tileGrid.toggleToolBar();
    }else if(tileGridResult == 1)
    {
      gridStack[currentGridIndex] = tileGrid;
      
      currentGridIndex = (currentGridIndex == 0 ? gridStack.size()-1 : currentGridIndex - 1);
      tileGrid = gridStack[currentGridIndex];
    }else if(tileGridResult == 2)
    {
      // if we haven't reached our board limit
      if(gridStack.size() < 3) { gridStack.push_back(referenceGrid); }

      gridStack[currentGridIndex] = tileGrid;
      currentGridIndex = (currentGridIndex == gridStack.size()-1 ? 0 : currentGridIndex + 1);
      tileGrid = gridStack[currentGridIndex];
    }

    updateCameraRotation(camera,0.001f);
    BeginDrawing();
    ClearBackground(BLACK);
    updateBackground(camera,rotationFrameCount,sinWaveValue,backgroundOnlyMode);
    //DrawRectangle(0,0,100,100,RED);
    
    tileGrid.draw(mousePos);
    if((IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_SPACE)) || IsKeyPressed(KEY_SPACE))
    {
      tileGrid.randomize();
    }

    EndDrawing();

  }

  CloseWindow();

  return 0;
}
