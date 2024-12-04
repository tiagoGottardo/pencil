#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./classes/Polygon.h"

#include <QPainter>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->frame->connectWindow(&displayFile);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if(event->key() == Qt::Key_Return) QCoreApplication::quit();

  if(event->key() == Qt::Key_F) {
    printf("Display file: \n");
    for(Drawable* drawable : this->displayFile) 
      drawable->checkItself();
  }

  // Resize window
  if(event->key() == Qt::Key_R) { ui->frame->getWindow()->setSize(); }

  // Move window
  if(event->key() == Qt::Key_D) { ui->frame->getWindow()->move(Point(50, 0)); }
  if(event->key() == Qt::Key_A) { ui->frame->getWindow()->move(Point(-50, 0)); }
  if(event->key() == Qt::Key_W) { ui->frame->getWindow()->move(Point(0, -50)); }
  if(event->key() == Qt::Key_S) { ui->frame->getWindow()->move(Point(0, 50)); }

  // Rotate window
  if(event->key() == Qt::Key_Q) { ui->frame->getWindow()->rotate(-1); }
  if(event->key() == Qt::Key_E) { ui->frame->getWindow()->rotate(1); }
};

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_left_clicked() {
  if(displayFileIndex == 0) displayFileIndex = ((int) displayFile.size()) - 1; else displayFileIndex--;

  ui->label->setText(QString::fromStdString(displayFile[displayFileIndex]->name));
}

void MainWindow::on_right_clicked() {
  if(displayFileIndex == (int) displayFile.size() - 1) displayFileIndex = 0; else displayFileIndex++;

  ui->label->setText(QString::fromStdString(displayFile[displayFileIndex]->name));
}

void movePolygon(Ui::MainWindow *ui, Polygon* polygon, char x, char y) {
  bool ok;
  int num = ui->moveEdit->displayText().toInt(&ok);
  if(ok) polygon->move(Point((x - 1) * num, (y - 1) * num));
}

void MainWindow::on_moveRight_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  movePolygon(ui, polygon, 2, 1);
}

void MainWindow::on_moveLeft_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  movePolygon(ui, polygon, 0, 1);
}

void MainWindow::on_moveTop_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  movePolygon(ui, polygon, 1, 0);
}

void MainWindow::on_moveBottom_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  movePolygon(ui, polygon, 1, 2);
}

void MainWindow::on_upScale_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  bool ok;
  int num = ui->scaleEdit->displayText().toInt(&ok);
  if(ok) polygon->scale((100 + (double) num) / 100.0);
}

void MainWindow::on_downScale_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  bool ok;
  int num = ui->scaleEdit->displayText().toInt(&ok);
  if(ok) polygon->scale((100 - (double) num) / 100.0);
}

void MainWindow::on_clockRotation_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  bool ok;
  int num = ui->rotateEdit->displayText().toInt(&ok);
  if(ok) polygon->rotate(num);
}

void MainWindow::on_antiClockRotation_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[displayFileIndex]);
  bool ok;
  int num = ui->rotateEdit->displayText().toInt(&ok);
  if(ok) polygon->rotate(-num);
}

void MainWindow::on_createPolygon_clicked() {
  int size, sides;
  std::string name;
  bool okSize, okSides;
  size = ui->sizeEdit->displayText().toInt(&okSize);
  sides = ui->sidesEdit->displayText().toInt(&okSides);
  name = ui->nameEdit->displayText().toStdString();

  if(okSize && okSides) {
    displayFile.push_back(Polygon::createRegularPolygon(size, sides, Point(0, 0), name));
    displayFileIndex = (int) displayFile.size() - 1;
    ui->label->setText(QString::fromStdString(displayFile[displayFileIndex]->name));
  }
}
