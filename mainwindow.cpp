#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./classes/Polygon.h"
#include "./classes/Model.h"

#include <QPainter>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->frame->connectWindow(&displayFile);
  
  displayFile.push_back(make_unique<Model>(Model::createDonut(40)));

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::triggerRotate);
  timer->start(5); 
}

void MainWindow::triggerRotate() {
  for(unique_ptr<Drawable>& drawable : this->displayFile) {
    Model* model = dynamic_cast<Model*>(drawable.get());
    if(model) {
      model->rotateX(1);
      model->rotateY(2);
      model->rotateZ(4);
    }
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if(event->key() == Qt::Key_Return) QCoreApplication::quit();

  if(event->key() == Qt::Key_F) {
    printf("Display file: \n");
    for(unique_ptr<Drawable>& drawable : this->displayFile) 
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
