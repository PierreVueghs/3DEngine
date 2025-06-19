#pragma once

#include "vec3.h"
#include "olcPixelGameEngine.h"

class Triangle
{
public:
  Triangle(const vec3& p0, const vec3& p1, const vec3& p2);
  vec3 GetNormal() const;
  void Draw(olc::PixelGameEngine* engine) const;
  void Fill(olc::PixelGameEngine* engine) const;
  void SetFillColor(const olc::Pixel& color);
  void SetEdgeColor(const olc::Pixel& color);
  vec3 P0() const;
  vec3 P1() const;
  vec3 P2() const;
  void Translate(const vec3& translation);
  void Scale(const vec3& scale);

private:
  vec3 p[3];
  olc::Pixel fillColor_{ olc::WHITE };
  olc::Pixel edgeColor_{ olc::BLACK };
};

