#pragma once

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include <iomanip>

#include "Matrix.h"

class Point {
private:
  string name;

public:
  double x;
  double y;
  double z;

  Point(const string& name) : name(name), x(0), y(0), z(0) {}
  Point(double x = 0, double y = 0, double z = 0, const string& name = "Point") : name(name), x(x), y(y), z(z) {}

  string getName() const { return name; }

  string interface() const { return (ostringstream() << "(" << std::fixed << std::setprecision(2) << x << "; " << y << "; " << z << ")").str(); }

  void checkItself() const { printf("  %s: (%.2f, %.2f, %.2f)\n", name.c_str(), x, y, z); }

  Point& normalize() {
    double length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    x /= length;
    y /= length;
    z /= length;

    return *this;
  };

  Point& applyMatrix(Matrix matrix, bool divideByW = false) {
    double vector[4] = { x, y, z, 1 };
    double result[4] = { 0, 0, 0, 0 };

    for(size_t i = 0; i < MATRIX_SIZE; i++)
      for(size_t j = 0; j < MATRIX_SIZE; j++) 
        result[i] += matrix[i][j] * vector[j];   

    if(divideByW) {
      if(result[3] == 0) result[3] = 1e-7; 

      result[0] /= result[3]; 
      result[1] /= result[3]; 
      result[2] /= result[3];
    }

    x = result[0]; 
    y = result[1]; 
    z = result[2];

    return *this;
  }

  bool operator==(const Point& other) const { 
    double epsilon = 1e-2;

    Point c = *this - other;
    if((c.x >= 0 && c.x >= epsilon) || (c.x < 0 && -c.x >= epsilon)) return false;
    if((c.y >= 0 && c.y >= epsilon) || (c.y < 0 && -c.y >= epsilon)) return false;
    if((c.z >= 0 && c.z >= epsilon) || (c.z < 0 && -c.z >= epsilon)) return false;

    return true;
  }
  bool operator!=(const Point& other) const { return !(*this == other); }
  Point operator-() const { return Point(-x, -y, -z); }
  Point operator+(const Point& other) const { return Point(x + other.x, y + other.y, z + other.z); }
  Point& operator+=(const Point& other) { *this = *this + other; return *this; }
  Point operator-(const Point& other) const { return Point(x - other.x, y - other.y, z - other.z); }
  Point& operator-=(const Point& other) { *this = *this - other; return *this; }
  Point operator*(const double& num) const { return Point(x * num, y * num, z * num, name); }
};

using Vector = Point;
