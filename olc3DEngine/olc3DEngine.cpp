// olc3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define OLC_PGE_APPLICATION

//#include "olcConsoleGameEngine.h"
#include "olcPixelGameEngine.h"

using namespace std;
using namespace olc;

struct vec3d
{
  float x, y, z;
};

struct triangle
{
  vec3d p[3];
};

struct mesh
{
  vector<triangle> tris;
};

struct mat4x4
{
  float m[4][4] = { 0.f };
};

class olcEngine3D : public PixelGameEngine
{
public:
  olcEngine3D() 
  {
    sAppName = "3D Demo";
  }

  bool OnUserCreate() override
  {
    meshCube.tris = {
      // SOUTH
      { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

      // EAST                                                      
      { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
      { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

      // NORTH                                                     
      { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
      { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

      // WEST                                                      
      { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

      // TOP                                                       
      { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
      { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

      // BOTTOM                                                    
      { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
      { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
    };

    // Projection matrix
    float zNear{ 0.1f };
    float zFar{ 1000.f };
    float fov{ 90.f };
    float aspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
    float fovRad = 1.f / tanf(fov * 0.5f * 3.14159f / 180.f);

    projectionMatrix.m[0][0] = aspectRatio * fovRad;
    projectionMatrix.m[1][1] = fovRad;
    projectionMatrix.m[2][2] = zFar / (zFar - zNear);
    projectionMatrix.m[3][2] = (-zFar * zNear) / (zFar - zNear);
    projectionMatrix.m[2][3] = 1.f;
    projectionMatrix.m[3][3] = 0.f;

    return true;
  }

  bool OnUserUpdate(float elapsedTime) override
  {
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);
    // Draw triangles
    for (const auto& tri : meshCube.tris)
    {
      triangle projectedTriangle;
      MultiplyMatrixVector(tri.p[0], projectionMatrix, projectedTriangle.p[0]);
      MultiplyMatrixVector(tri.p[1], projectionMatrix, projectedTriangle.p[1]);
      MultiplyMatrixVector(tri.p[2], projectionMatrix, projectedTriangle.p[2]);

      // Scale into view
      projectedTriangle.p[0].x += 1.f;
      projectedTriangle.p[0].y += 1.f;
      projectedTriangle.p[1].x += 1.f;
      projectedTriangle.p[1].y += 1.f;
      projectedTriangle.p[2].x += 1.f;
      projectedTriangle.p[2].y += 1.f;

      projectedTriangle.p[0].x *= 0.5f * (float)ScreenWidth();
      projectedTriangle.p[0].y *= 0.5f * (float)ScreenHeight();
      projectedTriangle.p[1].x *= 0.5f * (float)ScreenWidth();
      projectedTriangle.p[1].y *= 0.5f * (float)ScreenHeight();
      projectedTriangle.p[2].x *= 0.5f * (float)ScreenWidth();
      projectedTriangle.p[2].y *= 0.5f * (float)ScreenHeight();

      DrawTriangle(projectedTriangle.p[0].x, projectedTriangle.p[0].y,
        projectedTriangle.p[1].x, projectedTriangle.p[1].y,
        projectedTriangle.p[2].x, projectedTriangle.p[2].y,
        olc::WHITE);
        //PIXEL_SOLID, FG_WHITE);
    }

    return true;
  }

private:
  mesh meshCube;
  mat4x4 projectionMatrix;

  void MultiplyMatrixVector(const vec3d& input, const mat4x4& projectionMatrix, vec3d& output)
  {
    output.x = input.x * projectionMatrix.m[0][0] + input.y * projectionMatrix.m[1][0] + input.z * projectionMatrix.m[2][0] + projectionMatrix.m[3][0];
    output.y = input.x * projectionMatrix.m[0][1] + input.y * projectionMatrix.m[1][1] + input.z * projectionMatrix.m[2][1] + projectionMatrix.m[3][1];
    output.z = input.x * projectionMatrix.m[0][2] + input.y * projectionMatrix.m[1][2] + input.z * projectionMatrix.m[2][2] + projectionMatrix.m[3][2];
    float w = input.x * projectionMatrix.m[0][3] + input.y * projectionMatrix.m[1][3] + input.z * projectionMatrix.m[2][3] + projectionMatrix.m[3][3];
    if (w != 0.f)
    {
      output.x /= w;
      output.y /= w;
      output.z /= w;
    }
  }
};

int main()
{
  olcEngine3D demo;
  if (demo.Construct(256, 240, 2, 2))
    demo.Start();

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
