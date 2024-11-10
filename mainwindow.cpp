#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./classes/Polygon.h"

#include <QPainter>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  displayFile = std::vector<Drawable*>();

  Polygon* triangle = Polygon::createRegularPolygon(100, 3, Point(100, 100), "Ronaldinho");
  Polygon* triangle2 = Polygon::createRegularPolygon(100, 3, Point(100, 100), "Alan Turing");
  Polygon* triangle3 = Polygon::createRegularPolygon(100, 3, Point(100, 100), "Robert");

  triangle->redimensionXY(1.2);
  triangle2->rotate(90);
  triangle3->move(Point(50, 50));


  displayFile.push_back(triangle);
  displayFile.push_back(triangle2);
  displayFile.push_back(triangle3);

  ui->frame->setDisplayFile(&displayFile);
  ui->frame->setDisplayIndex(dfIndex = 0);
  ui->label->setText(QString::fromStdString(displayFile[dfIndex]->name));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::paintEvent(QPaintEvent *event) {
  event->ignore();
  QPainter painter(this);
  QPen pen(Qt::red, 3);
  pen.setWidth(1);
  painter.setPen(pen);

  for(int i = 0; i < (int) displayFile.size(); i++){
    if(i == dfIndex) {
      pen.setColor(Qt::white);
      painter.setPen(pen);
      displayFile[i]->draw(&painter);
      pen.setColor(Qt::red);
      painter.setPen(pen);
    } else
    displayFile[i]->draw(&painter);
  }
}

void MainWindow::on_left_clicked() {
  if(dfIndex == 0) dfIndex = ((int) displayFile.size()) - 1; else dfIndex--;

  ui->frame->setDisplayIndex(dfIndex);
  ui->label->setText(QString::fromStdString(displayFile[dfIndex]->name));
  ui->frame->update();
}

void MainWindow::on_right_clicked() {
  if(dfIndex == (int) displayFile.size() - 1) dfIndex = 0; else dfIndex++;

  ui->frame->setDisplayIndex(dfIndex);
  ui->label->setText(QString::fromStdString(displayFile[dfIndex]->name));
  ui->frame->update();
}
