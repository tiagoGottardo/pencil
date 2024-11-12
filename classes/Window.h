#ifndef WINDOW_H
#define WINDOW_H

#include <QFrame>
#include <QPainter>

#include "./Point.h"
#include "./Viewport.h"
#include "./Polygon.h"

class Window {
public:
  uint width, height;
  Point position;
  std::vector<Drawable*> *displayFile;
  std::vector<Drawable*> *draws;

  Window(uint width, uint height, std::vector<Drawable*>* displayFile) {
    this->width = width;
    this->height = height;
    this->displayFile = displayFile;
    this->draws = new std::vector<Drawable*>();
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
    this->draws->clear();
    for(sizet i = 0; i < this->displayFile->size(); i++) {
      Polygon* iterator = dynamic_cast<Polygon*>((*this->displayFile)[i]);
      this->draws->push_back(new Polygon(iterator->clone()));
    }

    Polygon* polygon;
    for(sizet i = 0; i < this->draws->size(); i++) {
      polygon = dynamic_cast<Polygon*>((*this->draws)[i]);
      polygon->ref = polygon->ref - this->position;
      std::vector<Point*> points = polygon->getPoints();
      for(sizet j = 0; j < points.size(); j++) 
        *points[j] = *points[j] + polygon->ref;

      polygon->scale(viewport->width / this->width, viewport->height / this->height);
      std::vector<Point*> ref = { &polygon->ref };
      polygon->scale(viewport->width / this->width, viewport->height / this->height, ref);

      for(sizet j = 0; j < points.size(); j++) 
        *points[j] = *points[j] - polygon->ref;
    }

    viewport->setDisplayFile(this->draws);
    viewport->update();
  }
};

#endif
