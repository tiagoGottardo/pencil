#pragma once

using namespace std;

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include <memory>

class Point;
class Matrix;
class Polygon;

class Drawable {
public:
  virtual string getName() const = 0;
  virtual vector<Polygon> getPolygons() const = 0;
  virtual Matrix getMatrix() const = 0;
  virtual void checkItself() const = 0;
  virtual ~Drawable() = default;
};

using DisplayFile = vector<unique_ptr<Drawable>>;
