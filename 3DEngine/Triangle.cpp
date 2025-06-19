#include "Triangle.h"

using namespace olc;

Triangle::Triangle(const vec3& p0, const vec3& p1, const vec3& p2)
{
  p[0] = p0;
  p[1] = p1;
  p[2] = p2;
}

vec3 Triangle::GetNormal() const
{
  vec3 line1(p[1].x() - p[0].x(), p[1].y() - p[0].y(), p[1].z() - p[0].z());
  vec3 line2(p[2].x() - p[0].x(), p[2].y() - p[0].y(), p[2].z() - p[0].z());
  vec3 normal{ line1.CrossProduct(line2) };

  double length{ sqrt(normal.x() * normal.x() + normal.y() * normal.y() + normal.z() * normal.z()) };
  normal /= length;
  return normal;
}

void Triangle::Draw(PixelGameEngine* engine) const
{
  engine->DrawTriangle(static_cast<int32_t>(p[0].x()), static_cast<int32_t>(p[0].y()),
    static_cast<int32_t>(p[1].x()), static_cast<int32_t>(p[1].y()),
    static_cast<int32_t>(p[2].x()), static_cast<int32_t>(p[2].y()),
    edgeColor_);
}

void Triangle::Fill(PixelGameEngine* engine) const
{
  engine->FillTriangle(static_cast<int32_t>(p[0].x()), static_cast<int32_t>(p[0].y()),
    static_cast<int32_t>(p[1].x()), static_cast<int32_t>(p[1].y()),
    static_cast<int32_t>(p[2].x()), static_cast<int32_t>(p[2].y()),
    fillColor_);
}

void Triangle::SetFillColor(const olc::Pixel& color)
{
  fillColor_ = color;
}

void Triangle::SetEdgeColor(const olc::Pixel& color)
{
  edgeColor_ = color;
}

vec3 Triangle::P0() const
{
  return p[0];
}

vec3 Triangle::P1() const
{
  return p[1];
}

vec3 Triangle::P2() const
{
  return p[2];
}

void Triangle::Translate(const vec3& translation)
{
  p[0].Translate(translation);
  p[1].Translate(translation);
  p[2].Translate(translation);
}

void Triangle::Scale(const vec3& scale)
{
  p[0].Scale(scale);
  p[1].Scale(scale);
  p[2].Scale(scale);
}
