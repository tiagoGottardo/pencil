#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QFrame>
#include <QPainter>
#include <QTimer>

#include "./Polygon.h"
#include "./Window.h"

#define WINDOW_INIT_HEIGHT 100
#define WINDOW_INIT_WIDTH 100

class Viewport : public QFrame {
private:
  Window* window;
  
  RectangleSize getSize() {
    return { (uint) this->width(), (uint) this->height() };
  }
  
public:
  explicit Viewport(QWidget *parent = nullptr) : QFrame(parent) {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Viewport::triggerRepaint);
    timer->start(100); // milliseconds to trigger
  }

  void connectWindow(std::vector<Drawable*>* displayFile) {
    if(!this->window)
      this->window = new Window(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, displayFile);
  }

  void connectWindow(Window* window) {
    if(!this->window)
      this->window = window;
  }

  Window* getWindow() { 
    return (this->window) ? this->window : nullptr; 
  }

protected:
  void paintEvent(QPaintEvent *) override {
    if(!this->window) return;

    std::vector<Drawable*>* draws = this->window->transformViewport(this->getSize());

    QPainter painter(this);
    painter.setPen(Qt::black);

    for(int i = 0; i < (int) draws->size(); i++)
      (*draws)[i]->draw(&painter);
  }

private slots:
  void triggerRepaint() { this->update(); }
};

#endif
