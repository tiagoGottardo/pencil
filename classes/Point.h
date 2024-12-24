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
  double x;
  double y;
  double z;

  Point(double x = 0, double y = 0, double z = 0, const string& name = "Point") : name(name), x(x), y(y), z(z) {}

  string getName() const { return name; }

  void checkItself() const { printf("  %s: (%.2f, %.2f, %.2f)\n", name.c_str(), x, y, z); }

  Point& applyMatrix(Matrix matrix) {
    double vector[4] = { x, y, z, 1 };
    double result[4] = { 0, 0, 0, 0 };

    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++) 
        result[i] += matrix[i][j] * vector[j];   

    x = result[0]; 
    y = result[1]; 
    z = result[2];

    return *this;
  }

  bool operator==(const Point& other) const { return (x == other.x && y == other.y && z == other.z); }
  bool operator!=(const Point& other) const { return !(*this == other); }
  Point operator-() const { return Point(-x, -y, -z); }
  Point operator+(const Point& other) const { return Point(x + other.x, y + other.y, z + other.z); }
  Point& operator+=(const Point& other) { *this = *this + other; return *this; }
  Point operator-(const Point& other) const { return Point(x - other.x, y - other.y, z - other.z); }
  Point& operator-=(const Point& other) { *this = *this - other; return *this; }
  Point operator/(const double& num) const { return Point(x / num, y / num, z / num, name); }
};
