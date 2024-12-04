#ifndef WINDOW_H
#define WINDOW_H

#include <QFrame>
#include <QPainter>

#include "./Point.h"
#include "./Polygon.h"

typedef struct RectangleSize {
  uint width, height;
} RectangleSize;

class Window {
private:
  uint width, height;
  Point position;
  std::vector<Drawable*> *displayFile;

  std::vector<Drawable*> *cloneDisplayFile() {
    std::vector<Drawable*> *clone = new std::vector<Drawable*>();
    for(sizet i = 0; i < this->displayFile->size(); i++) {
      Polygon* iterator = dynamic_cast<Polygon*>((*this->displayFile)[i]);
      clone->push_back(new Polygon(iterator->clone()));
    }
    return clone;
  }

public:
  Window(uint width, uint height, std::vector<Drawable*>* displayFile) : width(width), height(height), displayFile(displayFile) {
    this->position = Point(0, 0);
  }

  void setSize(){
    if(this->width == 500 && this->height == 250) {
      this->width = 100; this->height = 100;
    } else {
      this->width = 500; this->height = 250;
    }
  }

  void move(Point to) { this->position = this->position + to; }

  std::vector<Drawable*>* transformViewport(RectangleSize viewportSize) {
    std::vector<Drawable*> *draws = this->cloneDisplayFile();

    Polygon* polygon;
    for(sizet i = 0; i < draws->size(); i++) {
      polygon = dynamic_cast<Polygon*>((*draws)[i]);

      polygon->ref -= this->position;

      std::vector<Point*> points = polygon->getPoints();

      for(sizet j = 0; j < points.size(); j++) 
        *points[j] += polygon->ref;

      polygon->scale(viewportSize.width / this->width, viewportSize.height / this->height);

      for(sizet j = 0; j < points.size(); j++) 
        *points[j] -= polygon->ref;

      polygon->setRef();
    }

    return draws;
  }
};

#endif
