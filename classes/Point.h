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

  Point() : Point(0, 0, 0, "Point") {} 
  Point(int32_t x, int32_t y) : Point(x, y, 0, "Point") {} 
  Point(int32_t x, int32_t y, const std::string& name) : Point(x, y, 0, name) {} 
  Point(int32_t x, int32_t y, int32_t z) : Point(x, y, z, "Point") {} 
  Point(int32_t x, int32_t y, int32_t z, const std::string& name) : Drawable(name), x(x), y(y), z(z) {}

  void draw(QPainter* painter) const override { painter->drawPoint(x, y); }

  void checkItself() const override { printf("%s(%d, %d, %d)", name.c_str(), x, y, z); }

  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y && z == other.z);
  }

  bool operator!=(const Point& other) const { return !(*this == other); }

  Matrix toMatrix() { 
    return Matrix(1, 3, {{0, 0, (double) x}, {0, 1, (double) y}, {0, 2, (double) z}}); 
  }

  Point operator=(const Matrix& other) const {
    return Point(
      static_cast<int>(std::round(other[0][0])), 
      static_cast<int>(std::round(other[1][0])), 
      static_cast<int>(std::round(other[2][0])) 
    );
  }

  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y, z + other.z);
  }

  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y, z - other.z);
  }
};

#endif
