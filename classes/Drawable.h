#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QPainter>

class Drawable {
public:
  virtual void draw(QPainter* painter) const = 0;
  virtual void checkItself() const = 0;
};

#endif
