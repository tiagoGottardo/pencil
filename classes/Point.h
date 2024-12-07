#pragma once

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include "Matrix.h"

class Point {
private:
  string name;

public:
  int x;
  int y;
  int z;

  Point(int x = 0, int y = 0, int z = 0, const string& name = "Point") : name(name), x(x), y(y), z(z) {}

  string getName() const { return name; }

  void checkItself() const { printf("  %s: (%d, %d, %d)\n", name.c_str(), x, y, z); }

  Matrix toMatrix() { return Matrix({ {(double) x}, {(double) y}, {(double) z}, {1}, }); }

  void applyMatrix(Matrix matrix) { *this = matrix * this->toMatrix(); }

  Point& operator=(const Matrix& other) {
    if(other.getHeight() < 3) throw invalid_argument("Invalid Matrix!");

    x = round(other[0][0]); 
    y = round(other[1][0]); 
    z = round(other[2][0]);

    return *this;
  }

  bool operator==(const Point& other) const { return (x == other.x && y == other.y && z == other.z); }
  bool operator!=(const Point& other) const { return !(*this == other); }
  Point operator-() const { return Point(-x, -y, -z); }
  Point operator+(const Point& other) const { return Point(x + other.x, y + other.y, z + other.z); }
  Point& operator+=(const Point& other) { *this = *this + other; return *this; }
  Point operator-(const Point& other) const { return Point(x - other.x, y - other.y, z - other.z); }
  Point& operator-=(const Point& other) { *this = *this - other; return *this; }
  Point operator/(const int& num) const { return Point(x / num, y / num, z / num, name); }
};
