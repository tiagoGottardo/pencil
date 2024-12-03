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
    delete this->displayFile;
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

    QPainter painter(this);
    QPen pen(Qt::black);
    painter.setPen(pen);

    for(int i = 0; i < (int) (*displayFile).size(); i++){
      if(i == displayIndex) {
        painter.setPen(Qt::red);
        (*displayFile)[i]->draw(&painter);
        painter.setPen(Qt::black);
      } else (*displayFile)[i]->draw(&painter);
    }
  }

private:
  int displayIndex;
  std::vector<Drawable*> *displayFile;
};

#endif
