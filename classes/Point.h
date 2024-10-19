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

  Point(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) { name = "Dot"; };
  Point(int32_t x, int32_t y, int32_t z, std::string name) : x(x), y(y), z(z), name(name) {};

  void draw(QPainter* painter) const override {
    printf("Drawing dot now");
  }

  void checkItself() const override { printf("%s(%d, %d, %d)", name.c_str(), x, y, z); };

  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y && z == other.z);
  }

  bool operator!=(const Point& other) const {
    return !(*this == other);
  }
};

#endif
