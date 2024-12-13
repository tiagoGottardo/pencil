#include "mainwindow.h"

#include <QApplication>
#include <QPainter>
#include <QWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.showFullScreen();
  return app.exec();
}
