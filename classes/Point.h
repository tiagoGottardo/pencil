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
private:
  string name;

public:
  int32_t x;
  int32_t y;
  int32_t z;

  Point(int32_t x = 0, int32_t y = 0, int32_t z = 0, const string& name = "Point") : name(name), x(x), y(y), z(z) {}

  string getName() const override { return name; }

  vector<Line> getLines() const override { return vector<Line>(); }

  void draw(QPainter* painter) const override { painter->drawPoint(x, y); }

  void checkItself() const override { printf("  %s: (%d, %d, %d)\n", name.c_str(), x, y, z); }
  bool operator==(const Point& other) const { return (x == other.x && y == other.y && z == other.z); }
  bool operator!=(const Point& other) const { return !(*this == other); }

  Matrix toMatrix() { return Matrix({ {(double) x}, {(double) y}, {(double) z}, {1}, }); }

  Point& operator=(const Matrix& other) {
    if(other.getHeight() < 3) throw invalid_argument("Invalid Matrix!");

    this->name = "Point";
    x = round(other[0][0]); 
    y = round(other[1][0]); 
    z = round(other[2][0]);

    return *this;
  }

  Point& applyMatrix(Matrix matrix) { *this = matrix * this->toMatrix(); return *this; }

  void static applyMatrix(Point* point, Matrix matrix) { *point = matrix * point->toMatrix(); }

  Point operator-() const { return Point(-x, -y, -z); }
  Point operator+(const Point& other) const { return Point(x + other.x, y + other.y, z + other.z); }
  Point& operator+=(const Point& other) { *this = *this + other; return *this; }
  Point operator-(const Point& other) const { return Point(x - other.x, y - other.y, z - other.z); }
  Point& operator-=(const Point& other) { *this = *this - other; return *this; }

  Point operator/(const int& num) const { return Point(x / num, y / num, z / num, name); }
};

#endif
