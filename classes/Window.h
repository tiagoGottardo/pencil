#ifndef WINDOW_H
#define WINDOW_H

#include <QFrame>
#include <QPainter>

#include "./Point.h"
#include "./Viewport.h"
#include "./Polygon.h"

class Window {
private:
  std::vector<Drawable*> *cloneDisplayFile() {
    std::vector<Drawable*> *clone = new std::vector<Drawable*>();
    for(sizet i = 0; i < this->displayFile->size(); i++) {
      Polygon* iterator = dynamic_cast<Polygon*>((*this->displayFile)[i]);
      clone->push_back(new Polygon(iterator->clone()));
    }
    return clone;
  }

public:
  uint width, height;
  Point position;
  std::vector<Drawable*> *displayFile;

  Window(uint width, uint height, std::vector<Drawable*>* displayFile) {
    this->width = width;
    this->height = height;
    this->displayFile = displayFile;
    position = Point(0, 0, 0);
  }

  void setSize(){
    if(this->width == 500 && this->height == 250) {
      this->width = 100; this->height = 100;
    } else {
      this->width = 500; this->height = 250;
    }
  }

  void move(Point to) { this->position = this->position + to; }

  void transformViewport(Viewport* viewport) {
    std::vector<Drawable*> *draws = this->cloneDisplayFile();

    Polygon* polygon;
    for(sizet i = 0; i < draws->size(); i++) {
      polygon = dynamic_cast<Polygon*>((*draws)[i]);

      polygon->ref -= this->position;

      std::vector<Point*> points = polygon->getPoints();

      for(sizet j = 0; j < points.size(); j++) 
        *points[j] += polygon->ref;

      polygon->scale(viewport->width / this->width, viewport->height / this->height);

      for(sizet j = 0; j < points.size(); j++) 
        *points[j] -= polygon->ref;

      polygon->setRef();
    }

    viewport->setDisplayFile(draws);
    viewport->update();
  }
};

#endif
