#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"
#include <math.h>

void cameraSetUpFunc(Camera3D& camera)
{
  camera.position = {3.0f,13.0f,3.0f};
  //camera.position = {13.0f,23.0f,13.0f};
  camera.target   = {0.0f,5.0f,0.0f};
  camera.up       = {0.0f,1.0f,0.0f};
  camera.fovy = 45.0f;
}

void updateCameraRotation(Camera3D& camera, const float& angle)
{
  Matrix rotation = MatrixRotate(GetCameraUp(&camera), angle);
  Vector3 view = Vector3Subtract(camera.position, camera.target);
  view = Vector3Transform(view, rotation);
  camera.position = Vector3Add(camera.target, view);
}
