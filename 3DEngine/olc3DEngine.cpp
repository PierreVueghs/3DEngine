// olc3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <strstream>
#include <vector>
#define OLC_PGE_APPLICATION

//#include "olcConsoleGameEngine.h"
#include "olcPixelGameEngine.h"
#include "vec3.h"
#include "constants.h"
#include "Triangle.h"

using namespace std;
using namespace olc;

struct mesh
{
  vector<Triangle> tris;

  bool LoadFromObjectFile(const std::string& fileName)
  {
    ifstream file(fileName);
    if (!file.is_open())
      return false;

    // Local cache of vertices
    vector<vec3> vertices;

    while (!file.eof())
    {
      char line[128];
      file.getline(line, 128);

      strstream s;
      s << line;
      char junk; // First character of each line, being '#', 'v' or 'f'

      // Reading vertices
      if (line[0] == 'v')
      {
        double x, y, z;
        s >> junk >> x >> y >> z;
        vec3 vertex{ x, y, z };
        vertices.push_back(vertex);
      }
      else if (line[0] == 'f')
      {
        int f[3];
        s >> junk >> f[0] >> f[1] >> f[2];
        tris.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
      }
    }

    return true;
  }
};

struct mat4x4
{
  double m[4][4] = { 0. };
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
    meshCube.LoadFromObjectFile("VideoShip.obj");
    //meshCube.tris = {
    //  // SOUTH
    //  { vec3(0.0, 0.0, 0.0),    vec3(0.0, 1.0, 0.0),    vec3(1.0, 1.0, 0.0) },
    //  { vec3(0.0, 0.0, 0.0),    vec3(1.0, 1.0, 0.0),    vec3(1.0, 0.0, 0.0)},

    //  // EAST                                                      
    //  { vec3(1.0, 0.0, 0.0),    vec3(1.0, 1.0, 0.0),    vec3(1.0, 1.0, 1.0)},
    //  { vec3(1.0, 0.0, 0.0),    vec3(1.0, 1.0, 1.0),    vec3(1.0, 0.0, 1.0)},

    //  // NORTH                                                     
    //  { vec3(1.0, 0.0, 1.0),    vec3(1.0, 1.0, 1.0),    vec3(0.0, 1.0, 1.0)},
    //  { vec3(1.0, 0.0, 1.0),    vec3(0.0, 1.0, 1.0),    vec3(0.0, 0.0, 1.0)},

    //  // WEST                                                      
    //  { vec3(0.0, 0.0, 1.0),    vec3(0.0, 1.0, 1.0),    vec3(0.0, 1.0, 0.0)},
    //  { vec3(0.0, 0.0, 1.0),    vec3(0.0, 1.0, 0.0),    vec3(0.0, 0.0, 0.0)},

    //  // TOP                                                       
    //  { vec3(0.0, 1.0, 0.0),    vec3(0.0, 1.0, 1.0),    vec3(1.0, 1.0, 1.0)},
    //  { vec3(0.0, 1.0, 0.0),    vec3(1.0, 1.0, 1.0),    vec3(1.0, 1.0, 0.0)},

    //  // BOTTOM                                                    
    //  { vec3(1.0, 0.0, 1.0),    vec3(0.0, 0.0, 1.0),    vec3(0.0, 0.0, 0.0)},
    //  { vec3(1.0, 0.0, 1.0),    vec3(0.0, 0.0, 0.0),    vec3(1.0, 0.0, 0.0)},
    //};

    // Projection matrix
    double zNear{ 0.1 };
    double zFar{ 1000. };
    double fov{ 90. };
    double aspectRatio = (double)ScreenHeight() / (double)ScreenWidth();
    double fovRad = 1.f / tan(0.5 * degrees_to_radians(fov));

    projectionMatrix.m[0][0] = aspectRatio * fovRad;
    projectionMatrix.m[1][1] = fovRad;
    projectionMatrix.m[2][2] = zFar / (zFar - zNear);
    projectionMatrix.m[3][2] = (-zFar * zNear) / (zFar - zNear);
    projectionMatrix.m[2][3] = 1.;
    projectionMatrix.m[3][3] = 0.;

    return true;
  }

  bool OnUserUpdate(float elapsedTime) override
  {
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

    mat4x4 zRotation, xRotation;
    theta_ += 1. * (double)elapsedTime;

    // Rotation Z
    zRotation.m[0][0] = cos(theta_);
    zRotation.m[0][1] = sin(theta_);
    zRotation.m[1][0] = -sin(theta_);
    zRotation.m[1][1] = cos(theta_);
    zRotation.m[2][2] = 1.f;
    zRotation.m[3][3] = 1.f;

    // Rotation X
    xRotation.m[0][0] = 1.f;
    xRotation.m[1][1] = cos(theta_ * 0.5f);
    xRotation.m[1][2] = sin(theta_ * 0.5f);
    xRotation.m[2][1] = -sin(theta_ * 0.5f);
    xRotation.m[2][2] = cos(theta_ * 0.5f);
    xRotation.m[3][3] = 1.f;

    vector<Triangle> trianglesToRaster;

    // Draw triangles
    for (const auto& tri : meshCube.tris)
    {
      // Rotation around Z-axis
      Triangle zRotatedTriangle{ MultiplyMatrixVector(tri.P0(), zRotation),
        MultiplyMatrixVector(tri.P1(), zRotation),
        MultiplyMatrixVector(tri.P2(), zRotation) };

      // Rotation around X-axis
      Triangle xRotatedTriangle{ MultiplyMatrixVector(zRotatedTriangle.P0(), xRotation),
        MultiplyMatrixVector(zRotatedTriangle.P1(), xRotation),
        MultiplyMatrixVector(zRotatedTriangle.P2(), xRotation)};

      // Translation - Offset into the screen
      Triangle translatedTriangle{ xRotatedTriangle };
      translatedTriangle.Translate(vec3(0., 0., 8.));

      vec3 normal{ translatedTriangle.GetNormal() };

      //if (normal.z() < 0.)
      if ((translatedTriangle.P0() - camera_).DotProduct(normal) < 0.)
      {
        // Illumination
        vec3 lightDirection{ 0., 0., -1 };
        lightDirection.Normalize();
        double dp{ lightDirection.DotProduct(normal) };
        Pixel color{ WHITE };
        color.r *= dp;
        color.g *= dp;
        color.b *= dp;

        // Project triangle from 3D --> 2D
        Triangle projectedTriangle{ MultiplyMatrixVector(translatedTriangle.P0(), projectionMatrix),
          MultiplyMatrixVector(translatedTriangle.P1(), projectionMatrix),
          MultiplyMatrixVector(translatedTriangle.P2(), projectionMatrix)};

        // Scale into view
        vec3 move{ 1., 1., 0. };
        projectedTriangle.Translate(move);

        vec3 scale{ 0.5 * (double)ScreenWidth(), 0.5 * (double)ScreenHeight(), 1. };
        projectedTriangle.Scale(scale);

        projectedTriangle.SetFillColor(color);

        trianglesToRaster.push_back(projectedTriangle);
      }
    }

    // Sort triangles from back to front (painter algorithm)
    sort(begin(trianglesToRaster), end(trianglesToRaster),
      [](const Triangle& t1, const Triangle& t2) {
        // Compare mid-points (x3)
        double z1{ t1.P0().z() + t1.P1().z() + t1.P2().z() };
        double z2{ t2.P0().z() + t2.P1().z() + t2.P2().z() };
        return z2 < z1;
      });

    for (const auto& triangle : trianglesToRaster)
    {
      triangle.Fill(this);
      // For debug
      //triangle.Draw(this);
    }

    return true;
  }

private:
  mesh meshCube;
  mat4x4 projectionMatrix;
  double theta_{ 0. };
  vec3 camera_{ 0., 0., 0. };

  vec3 MultiplyMatrixVector(const vec3& input, const mat4x4& projectionMatrix)
  {
    double x = input.x() * projectionMatrix.m[0][0] + input.y() * projectionMatrix.m[1][0] + input.z() * projectionMatrix.m[2][0] + projectionMatrix.m[3][0];
    double y = input.x() * projectionMatrix.m[0][1] + input.y() * projectionMatrix.m[1][1] + input.z() * projectionMatrix.m[2][1] + projectionMatrix.m[3][1];
    double z = input.x() * projectionMatrix.m[0][2] + input.y() * projectionMatrix.m[1][2] + input.z() * projectionMatrix.m[2][2] + projectionMatrix.m[3][2];
    double w = input.x() * projectionMatrix.m[0][3] + input.y() * projectionMatrix.m[1][3] + input.z() * projectionMatrix.m[2][3] + projectionMatrix.m[3][3];
    if (w != 0.)
    {
      x /= w;
      y /= w;
      z /= w;
    }
    return vec3(x, y, z);
  }
};

int main()
{
  olcEngine3D demo;
  if (demo.Construct(256, 240, 2, 2))
    demo.Start();

  std::cout << "Hello World!\n";
}

