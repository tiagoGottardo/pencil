#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./classes/Polygon.h"

#include <QPainter>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), window(Window(100, 100, &displayFile)) {
  ui->setupUi(this);
  // ui->frame->setDisplayFile(&displayFile);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if(event->key() == Qt::Key_Return) QCoreApplication::quit();

  if(event->key() == Qt::Key_F) {
    printf("Display file: \n");
    for(Drawable* drawable : (*window.displayFile)) 
      drawable->checkItself();

    printf("\nWindow file: \n");
    for(Drawable* drawable : (*window.draws)) 
      drawable->checkItself();
  }

  if(event->key() == Qt::Key_R) { window.setSize(); window.transformViewport(ui->frame); }

  if(event->key() == Qt::Key_D) { window.move(Point(50, 0)); window.transformViewport(ui->frame); }
  if(event->key() == Qt::Key_A) { window.move(Point(-50, 0)); window.transformViewport(ui->frame); }
  if(event->key() == Qt::Key_W) { window.move(Point(0, -50)); window.transformViewport(ui->frame); }
  if(event->key() == Qt::Key_S) { window.move(Point(0, 50)); window.transformViewport(ui->frame); }
};

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_left_clicked() {
  if(dfIndex == 0) dfIndex = ((int) displayFile.size()) - 1; else dfIndex--;

  ui->frame->setDisplayIndex(dfIndex);
  ui->label->setText(QString::fromStdString(displayFile[dfIndex]->name));
  window.transformViewport(ui->frame);
}

void MainWindow::on_right_clicked() {
  if(dfIndex == (int) displayFile.size() - 1) dfIndex = 0; else dfIndex++;

  ui->frame->setDisplayIndex(dfIndex);
  ui->label->setText(QString::fromStdString(displayFile[dfIndex]->name));
  window.transformViewport(ui->frame);
}

void movePolygon(Ui::MainWindow *ui, Window w, Polygon* polygon, char x, char y) {
  bool ok;
  int num = ui->moveEdit->displayText().toInt(&ok);
  if(ok) polygon->move(Point((x - 1) * num, (y - 1) * num, 0));
  w.transformViewport(ui->frame);
}

void MainWindow::on_moveRight_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, window, polygon, 2, 1);
}

void MainWindow::on_moveLeft_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, window, polygon, 0, 1);
}

void MainWindow::on_moveTop_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, window, polygon, 1, 0);
}

void MainWindow::on_moveBottom_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  movePolygon(ui, window, polygon, 1, 2);
}

void MainWindow::on_upScale_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->scaleEdit->displayText().toInt(&ok);
  if(ok) polygon->scale((100 + (double) num) / 100.0);
  window.transformViewport(ui->frame);
}

void MainWindow::on_downScale_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->scaleEdit->displayText().toInt(&ok);
  if(ok) polygon->scale((100 - (double) num) / 100.0);
  window.transformViewport(ui->frame);
}

void MainWindow::on_clockRotation_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->rotateEdit->displayText().toInt(&ok);
  if(ok) polygon->rotate(num);
  window.transformViewport(ui->frame);
}

void MainWindow::on_antiClockRotation_clicked() {
  Polygon* polygon = dynamic_cast<Polygon*>(displayFile[dfIndex]);
  bool ok;
  int num = ui->rotateEdit->displayText().toInt(&ok);
  if(ok) polygon->rotate(-num);
  window.transformViewport(ui->frame);
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
    window.transformViewport(ui->frame);
  }
}

