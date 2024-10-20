#ifndef POINT_H
#define POINT_H

#include "Drawable.h"

#include <QPainter>
#include <iostream>
#include <sstream>

class Point : public Drawable {
public:
  int32_t x;
  int32_t y;
  int32_t z;

  std::string name;

  Point(int32_t x, int32_t y) : Point(x, y, 0, "Point") {} 
  Point(int32_t x, int32_t y, const std::string& name) : Point(x, y, 0, name) {} 
  Point(int32_t x, int32_t y, int32_t z) : Point(x, y, z, "Point") {} 
  Point(int32_t x, int32_t y, int32_t z, const std::string& name) : x(x), y(y), z(z), name(name) {}

  void draw(QPainter* painter) const override { painter->drawPoint(x, y); }

  void checkItself() const override { printf("%s(%d, %d, %d)", name.c_str(), x, y, z); }

  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y && z == other.z);
  }

  bool operator!=(const Point& other) const {
    return !(*this == other);
  }
};

#endif
