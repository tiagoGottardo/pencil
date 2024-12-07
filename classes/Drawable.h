#ifndef DRAWABLE_H
#define DRAWABLE_H

using namespace std;

#ifndef TEST_MODE
#include <QPainter>
#else 
#include "../tests/mocks/QPainterMock.h"
#endif

class Line;

class Drawable {
public:
  virtual void draw(QPainter* painter) const = 0;
  virtual std::string getName() const = 0;
  virtual std::vector<Line> getLines() const = 0;
  virtual void checkItself() const = 0;
  virtual ~Drawable() = default;
};

#endif
