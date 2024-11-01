#ifndef POINT_H
#define POINT_H

#include "Drawable.h"
#include "Matrix.h"

#include <QPainter>

class Point : public Drawable {
public:
  int32_t x;
  int32_t y;
  int32_t z;

  std::string name;

  Point() : Point(0, 0, 0, "Point") {} 
  Point(int32_t x, int32_t y) : Point(x, y, 0, "Point") {} 
  Point(int32_t x, int32_t y, const std::string& name) : Point(x, y, 0, name) {} 
  Point(int32_t x, int32_t y, int32_t z) : Point(x, y, z, "Point") {} 
  Point(int32_t x, int32_t y, int32_t z, const std::string& name) : x(x), y(y), z(z), name(name) {}

  void draw(QPainter* painter) const override { painter->drawPoint(x, y); }

  void checkItself() const override { printf("%s(%d, %d, %d)", name.c_str(), x, y, z); }

  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y && z == other.z);
  }

  bool operator!=(const Point& other) const { return !(*this == other); }

  Matrix<int32_t> toMatrix() { 
    return Matrix<int32_t>(1, 3, {{0, 0, x}, {0, 1, y}, {0, 2, z}}); 
  }

  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y, z + other.z);
  }

};

#endif
