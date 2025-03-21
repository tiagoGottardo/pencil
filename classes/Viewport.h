#pragma once

#include <QFrame>
#include <QPainter>
#include <QTimer>

#include "./Factory.h"
#include "./Window.h"

#define WINDOW_INIT_WIDTH 1311
#define WINDOW_INIT_HEIGHT 761 
#define WINDOW_INIT_FAR 1300 

#define FRAME_WIDTH 1311
#define FRAME_HEIGHT 761 

// This class couldn't has tests ):
// cause it's related with QFrame

class Viewport : public QFrame {
private:
  Window* window;

  RectangleSize getFrameSize() { return { FRAME_WIDTH, FRAME_HEIGHT }; }

  Point center() { return Point(this->width() / 2., this->height() / 2.); }

  void draw(QPainter* painter, vector<Line>* lines) {
    for(Line line : *lines) line.draw(painter, (int) this->height());

    for(Line line : Factory::createFrame(FRAME_WIDTH, FRAME_HEIGHT, center(), "Frame")) 
      line.draw(painter, (int) this->height());
  }

public:
  explicit Viewport(QWidget *parent = nullptr) : QFrame(parent) {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Viewport::triggerRepaint);
    timer->start(5); // milliseconds to trigger
  }

  void connectWindow(DisplayFile* displayFile) { if(!window) window = new Window(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, WINDOW_INIT_FAR, displayFile); }

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
