#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include <variant>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  std::vector<Point> points;
  std::vector<Line>* lines = new std::vector<Line>();

  displayFile = new std::vector<Thing*>();

  Point* a = new Point(50, 50, 0);
  Point* b = new Point(100, 200, 0);
  Point* c = new Point(300, 300, 0);

  Line A(a, b);
  Line B(b, c);
  Line C(c, a);

  lines->push_back(A);
  lines->push_back(B);
  lines->push_back(C);

  Polygon* myPolygon = new Polygon(lines);

  displayFile->push_back(myPolygon);
  displayFile->push_back(a);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  QPen pen(Qt::white, 3);
  pen.setWidth(1);
  painter.setPen(pen);

  (*displayFile)[0]->checkItself();
  (*displayFile)[1]->checkItself();
}

void MainWindow::on_pBx1_clicked() {
  // myPolygon->redimensionXY(1.5);
  // myPolygon->checkItself();
  // update();
}

void MainWindow::on_pBx2_clicked() {
  // myPolygon->redimensionXY(0.6666666666);
  // myPolygon->checkItself();
  // update();
}
