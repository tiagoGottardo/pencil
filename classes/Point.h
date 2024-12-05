#ifndef POINT_H
#define POINT_H

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include "Drawable.h"
#include "Matrix.h"

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
    return Matrix({
      {(double) x},
      {(double) y},
      {(double) z},
      {1.0},
    }); 
  }

  void operator=(const Matrix& other) {
    if(other.getHeight() < 3) throw std::invalid_argument("Invalid Matrix!");

    if(other.getWidth() >= 3) {
      x = std::round(other[0][0]); 
      y = std::round(other[1][1]); 
      z = std::round(other[2][2]);
    } else {
      x = std::round(other[0][0]); 
      y = std::round(other[1][0]); 
      z = std::round(other[2][0]);
    }
  }

  void applyMatrix(Matrix matrix) {
    *this = matrix * toMatrix();
  }

  Point operator-() const { return Point(-x, -y, -z); }
  Point operator+(const Point& other) const { return Point(x + other.x, y + other.y, z + other.z); }
  Point& operator+=(const Point& other) { *this = *this + other; return *this; }
  Point operator-(const Point& other) const { return Point(x - other.x, y - other.y, z - other.z); }
  Point& operator-=(const Point& other) { *this = *this - other; return *this; }
};

#endif
