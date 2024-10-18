#ifndef THING_H
#define THING_H

#include <QPainter>

enum DrawMode{
  FILL,
  LINE
};

class Thing {
public:
  virtual ~Thing() = default;
  virtual void draw() const = 0;
  virtual void checkItself();
};

#endif
