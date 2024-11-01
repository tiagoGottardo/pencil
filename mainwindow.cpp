#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./classes/Polygon.h"


#include <QPainter>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  displayFile = std::vector<Drawable*>();

  Polygon* triangle = Polygon::createRegularPolygon(100, 3, Point(100, 100), "Triangle");
  // Polygon* quad = Polygon::createRegularPolygon(100, 4, Point(200, 200));
  // Polygon* pent = Polygon::createRegularPolygon(100, 5, Point(300, 300));
  // 
  // Polygon* myPolygon = new Polygon({
  //   {50, 50},
  //   {100, 200},
  //   {300, 300}
  // }, "Ronaldinho Soccer");

  triangle->rotate(30);

  displayFile.push_back(triangle);
  // displayFile.push_back(quad);
  // displayFile.push_back(pent);
  // displayFile.push_back(myPolygon);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::paintEvent(QPaintEvent *event) {
  event->ignore();
  QPainter painter(this);

  QPen pen(Qt::red, 3);
  pen.setWidth(1);
  painter.setPen(pen);

  for(Drawable* drawable : displayFile)
    drawable->draw(&painter);
}


void MainWindow::on_pBx1_clicked() {
  for(Drawable* drawable : displayFile)
    (dynamic_cast<Polygon*>(drawable))->redimensionXY(1.25);
  update();
}

void MainWindow::on_pBx2_clicked() {
  for(Drawable* drawable : displayFile)
    (dynamic_cast<Polygon*>(drawable))->redimensionXY(0.75);
  update();
}

