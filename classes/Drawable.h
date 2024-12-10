#pragma once

using namespace std;

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

#include <memory>

class Line;

class Drawable {
public:
  virtual string getName() const = 0;
  virtual vector<Line> getLines() const = 0;
  virtual void checkItself() const = 0;
  virtual ~Drawable() = default;
};

using DisplayFile = vector<unique_ptr<Drawable>>;
