#include <math.h>
#include "raylib.h"
#include "backgroundSystem.h"

void drawBackgroundGrid(const Camera3D& camera,int& rotationFrameCount,float& sinWaveValue)
{

  if(rotationFrameCount % 2 == 0)
  {
    rotationFrameCount = 0;
		sinWaveValue -= 0.03f;

		if(sinWaveValue <= 0) { sinWaveValue += 2 * PI; }
  }
  rotationFrameCount++;
  BeginMode3D(camera);
  DrawGrid(30, 5.0f);
  //DrawPlane(Vector3{0,0.9f,0},Vector2{13.0f,13.0f},BLACK);

	float spacing(1.0f);
	int squareWaveWidth(13);
	//float cubeWidth(0.5f);
	float cubeWidth(0.5f);

	for(int i=-squareWaveWidth + 1;i<squareWaveWidth - 1;++i)
	{
		for(int j=-squareWaveWidth + 1;j<squareWaveWidth - 1;++j)
		{
      Color currentColor
      {
        //static_cast<unsigned char>(130 + 125 * sinf(sqrt(((i+2.0f)*(i+2.0f))+((j+9.0f)*(j+1.0f))) + sinWaveValue)),
        static_cast<unsigned char>(130 + 125 * sinf(sqrt(((i+0.0f)*(i+0.0f))+((j+0.0f)*(j+0.0f))) + sinWaveValue)),
        static_cast<unsigned char>(130 + 125 * sinf(sqrt(((i+1.0f)*(i+5.0f))+((j+3.0f)*(j+2.0f))) + sinWaveValue)),
        static_cast<unsigned char>(130 + 125 * sinf(sqrt(((i+8.0f)*(i+2.0f))+((j+8.0f)*(j+4.0f))) + sinWaveValue)),
        //static_cast<unsigned char>(130 + 125 * sinf(sqrt((i*i)+(j*j)) + sinWaveValue)),
        //static_cast<unsigned char>(130 + 125 * sinf(sqrt((i*i)+(j*j)) + sinWaveValue)),
        255
      };

      for(int n=-1;n<=1;n+=1)
      {
        for(int m=-1;m<=1;m+=1)
        {
          DrawLine3D
          (
            Vector3
            {
              //(cubeWidth * i) * spacing + (0.5f * cubeWidth),
              (cubeWidth * i) * spacing,
              //5 + 0.2f * float(sqrt(abs((i*i) + (j*j)))),
              2 + sinf(abs(float(sqrt((i*i) + (j*j)))) + sinWaveValue),
              (cubeWidth * j) * spacing
            },
            Vector3
            {
              (cubeWidth * (i+n)) * spacing,
              //5 + 0.2f * float(sqrt(abs(((i + 1)*(i+1)) + (j*j)))) ,
              2 + sinf(abs(float(sqrt(((i+n)*(i+n)) + ((j+m)*(j+m))))) + sinWaveValue),
              (cubeWidth * (j+m)) * spacing
            },
            currentColor
            //BLACK
          );
        }
      }

		}
	}

  EndMode3D();
}


void updateBackground(const Camera3D& camera,int& rotationFrameCount,float& sinWaveValue,bool& backgroundOnlyMode)
{
	drawBackgroundGrid(camera,rotationFrameCount,sinWaveValue);
	if(IsKeyPressed(KEY_B)) { backgroundOnlyMode = !backgroundOnlyMode; }
}
