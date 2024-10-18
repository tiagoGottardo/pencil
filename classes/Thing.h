#ifndef THING_H
#define THING_H

#include <QPainter>

class Thing {
public:
  virtual void checkItself() const = 0;
  // virtual void draw() const = 0;
};

#endif
