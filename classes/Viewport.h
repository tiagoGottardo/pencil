#pragma once

#include <QFrame>
#include <QPainter>
#include <QTimer>

#include "./Polygon.h"
#include "./Window.h"

#define WINDOW_INIT_WIDTH 460
#define WINDOW_INIT_HEIGHT 230

#define FRAME_WIDTH 1511
#define FRAME_HEIGHT 1061 

// This class couldn't has tests ):
// cause it's related with QFrame

class Viewport : public QFrame {
private:
  Window* window;

  RectangleSize getFrameSize() { return { FRAME_WIDTH, FRAME_HEIGHT }; }

  Point center() { 
    return Point(this->width() / 2, this->height() / 2); 
  }

  Polygon frame() { 
    return Polygon::createRectangle(FRAME_WIDTH, FRAME_HEIGHT, center(), "Frame"); 
  }

  void draw(QPainter* painter, vector<Line>* lines) {
    for(Line line : *lines) line.draw(painter);
    for(Line line : frame().getLines()) line.draw(painter);
  }

public:
  explicit Viewport(QWidget *parent = nullptr) : QFrame(parent) {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Viewport::triggerRepaint);
    timer->start(100); // milliseconds to trigger
  }

  void connectWindow(DisplayFile* displayFile) {
    if(!window) window = new Window(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, displayFile);
  }

  void connectWindow(Window* window) { if(!window) window = window; }

  Window* getWindow() { return (window) ? window : nullptr; }

protected:
  void paintEvent(QPaintEvent *) override {
    if(!window) return;

    vector<Line> lines = window->transformViewport(getFrameSize(), center());

    QPainter painter(this);
    painter.setPen(Qt::black);

    draw(&painter, &lines);
  }

private slots:
  void triggerRepaint() { this->update(); }
};
