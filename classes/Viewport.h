#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QFrame>
#include <QPainter>
#include <QTimer>

#include "./Polygon.h"
#include "./Window.h"

#define WINDOW_INIT_HEIGHT 100
#define WINDOW_INIT_WIDTH 100

// This class couldn't has test ):
// cause it's related with QFrame

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
    if(!window) window = new Window(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, displayFile);
  }

  void connectWindow(Window* window) { if(!window) window = window; }

  Window* getWindow() { return (window) ? window : nullptr; }

  void debugWindow() {
    if(!window) { printf("No window connected yet.\n"); return; }

    std::vector<Line> draws = window->transformViewport(this->getSize());

    printf("Window connected\n");

    if(draws.size() == 0) { printf("No lines to draw\n"); return; }

    printf("Lines after displayFile clipping: \n");
    for(Line line : draws) {
      line.checkItself();
      printf("\n");
    }
    printf("\n");
  }

protected:
  void paintEvent(QPaintEvent *) override {
    if(!window) return;

    std::vector<Line> draws = window->transformViewport(this->getSize());

    QPainter painter(this);
    painter.setPen(Qt::black);

    for(int i = 0; i < (int) draws.size(); i++) 
      draws[i].draw(&painter);
    
  }

private slots:
  void triggerRepaint() { this->update(); }
};

#endif
