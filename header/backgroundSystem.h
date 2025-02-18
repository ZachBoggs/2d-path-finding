#ifndef BACKGROUNDSYSTEM_H
#define BACKGROUNDSYSTEM_H

#include "raylib.h"
#include "rcamera.h"

void drawBackgroundGrid(const Camera3D&,int&,float&);
void updateBackground(const Camera3D&,int&,float&,bool&);

#endif
