#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QFrame>
#include <QPainter>

#include "./Polygon.h"

class Viewport : public QFrame {
  // Q_OBJECT
  
public:
  uint width, height;

  void setDisplayFile(std::vector<Drawable*> *displayFile) {
    this->displayFile = displayFile;
  }

  void setDisplayIndex(int newIndex) {
    this->displayIndex = newIndex;
  }

  explicit Viewport(QWidget *parent = nullptr) : QFrame(parent), width(500), height(250) {}

protected:
  void paintEvent(QPaintEvent *) override {
    printf("paintEvent viewport\n");
    if (!displayFile) return;

    // Polygon* circle;
    // Polygon* iterator;
    QPainter painter(this);
    QPen pen(Qt::black);
    painter.setPen(pen);

    for(int i = 0; i < (int) (*displayFile).size(); i++){
      // iterator = dynamic_cast<Polygon*>((*displayFile)[i]);
      // circle = Polygon::createRegularPolygon(20, 12, iterator->ref);
      if(i == displayIndex) {
        painter.setPen(Qt::red);
        (*displayFile)[i]->draw(&painter);
        painter.setPen(Qt::black);
        // circle->draw(&painter);
      } else (*displayFile)[i]->draw(&painter);
    }
  }

private:
  int displayIndex;
  std::vector<Drawable*> *displayFile;
};

#endif
