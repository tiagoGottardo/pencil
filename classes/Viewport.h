#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QFrame>
#include <QPainter>
#include <QColor>

#include "./Polygon.h"

class Viewport : public QFrame {
private:
  int displayIndex;
  std::vector<Drawable*> *displayFile;

  void paint(Drawable* obj, QPainter* painter, QColor color) {
    painter->setPen(color);
    obj->draw(painter);
  };
  
public:
  uint width, height;

  void setDisplayFile(std::vector<Drawable*> *displayFile) {
    if(this->displayFile) delete this->displayFile;
    this->displayFile = displayFile;
  }

  void setDisplayIndex(int newIndex) {
    this->displayIndex = newIndex;
  }

  explicit Viewport(QWidget *parent = nullptr) : QFrame(parent), width(500), height(250) {}

protected:
  void paintEvent(QPaintEvent *) override {
    if (!displayFile) return;
    QPainter painter(this);

    for(int i = 0; i < (int) (*displayFile).size(); i++)
      if(i == displayIndex) {
        this->paint((*displayFile)[i], &painter, Qt::red);
      } else 
        this->paint((*displayFile)[i], &painter, Qt::black);
  }
};

#endif
