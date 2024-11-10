#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QPainter>

class Drawable {
public:
  std::string name;

  Drawable (std::string name) : name(name) {}
  virtual void draw(QPainter* painter) const = 0;
  virtual void checkItself() const = 0;
};

#endif
