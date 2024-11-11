#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./classes/Polygon.h"

#include <QPainter>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  displayFile = std::vector<Drawable*>();

  // Polygon* triangle = Polygon::createRegularPolygon(100, 3, Point(100, 100), "Ronaldinho");
  // Polygon* triangle2 = Polygon::createRegularPolygon(100, 3, Point(100, 100), "Alan Turing");
  // Polygon* triangle3 = Polygon::createRegularPolygon(100, 3, Point(100, 100), "Robert");

  // displayFile.push_back(triangle);
  // displayFile.push_back(triangle2);
  // displayFile.push_back(triangle3);

  ui->frame->setDisplayFile(&displayFile);
  ui->frame->setDisplayIndex(dfIndex = 0);
  if((int) displayFile.size() != 0)
    ui->label->setText(QString::fromStdString(displayFile[dfIndex]->name));
}

MainWindow::~MainWindow() { delete ui; }

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

void movePolygon(Ui::MainWindow *ui, Polygon* polygon, char x, char y) {
  bool ok;
  int num = ui->moveEdit->displayText().toInt(&ok);
  if(ok) polygon->move(Point((x - 1) * num, (y - 1) * num, 0));
  ui->frame->update();
}

void MainWindow::on_moveRight_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, polygon, 2, 1);
}

void MainWindow::on_moveLeft_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, polygon, 0, 1);
}

void MainWindow::on_moveTop_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, polygon, 1, 0);
}

void MainWindow::on_moveBottom_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, polygon, 1, 2);
}

void MainWindow::on_upScale_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->scaleEdit->displayText().toInt(&ok);
  if(ok) polygon->scale((100 + (double) num) / 100.0);
  ui->frame->update();
}

void MainWindow::on_downScale_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->scaleEdit->displayText().toInt(&ok);
  if(ok) polygon->scale((100 - (double) num) / 100.0);
  ui->frame->update();
}

void MainWindow::on_clockRotation_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->rotateEdit->displayText().toInt(&ok);
  if(ok) polygon->rotate(num);
  ui->frame->update();
}

void MainWindow::on_antiClockRotation_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->rotateEdit->displayText().toInt(&ok);
  if(ok) polygon->rotate(-num);
  ui->frame->update();
}

void MainWindow::on_createPolygon_clicked() {
  int size, sides;
  std::string name;
  bool okSize, okSides;
  size = ui->sizeEdit->displayText().toInt(&okSize);
  sides = ui->sidesEdit->displayText().toInt(&okSides);
  name = ui->nameEdit->displayText().toStdString();

  if(okSize && okSides) {
    displayFile.push_back(Polygon::createRegularPolygon(size, sides, Point(size / 2, size / 2), name));
    dfIndex = (int) displayFile.size() - 1;
    ui->label->setText(QString::fromStdString(displayFile[dfIndex]->name));
    ui->frame->update();
  }
}

