#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./classes/Polygon.h"
#include "./classes/Model.h"

#include <QPainter>
#include <QCursor>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), displayFileIndex(0) {
  ui->setupUi(this);
  ui->frame->connectWindow(&displayFile);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::triggerRotate);
  timer->start(5); 
}

void MainWindow::triggerRotate() {
  Window* w = ui->frame->getWindow();

  ui->windowInfo->setText(QString::fromStdString(w->interface()));
  
  ui->label->setText((displayFile.size() == 0) ? QString::fromStdString("") : QString::fromStdString(displayFile[displayFileIndex].get()->getName()));

  QPoint mouseLoc = QCursor::pos();

  if(this->cursor().shape() == Qt::BlankCursor) {
    QPoint center = ui->frame->rect().center();
    QPoint globalCenter = ui->frame->mapToGlobal(center);

    int x = mouseLoc.x() - globalCenter.x();
    int y = mouseLoc.y() - globalCenter.y();

    ui->frame->getWindow()->rotateX(y * 0.01);
    ui->frame->getWindow()->rotateY(x * 0.01);

    QCursor::setPos(globalCenter);
  }

  for(unique_ptr<Drawable>& drawable : this->displayFile) {
    Model* model = dynamic_cast<Model*>(drawable.get());
    if(model) 
      model->rotate(
        (model->fixedXRotation) ? 1 : 0, 
        (model->fixedYRotation) ? 2 : 0, 
        (model->fixedZRotation) ? 4 : 0);
  }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setCursor(Qt::BlankCursor);
  } else if (event->button() == Qt::RightButton) {
    setCursor(Qt::ArrowCursor);
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  Window* w = ui->frame->getWindow();

  if(event->key() == Qt::Key_Return) QCoreApplication::quit();

  if(event->key() == Qt::Key_F) {
    printf("Display file: \n");
    for(unique_ptr<Drawable>& drawable : this->displayFile) 
      drawable->checkItself();
  }

  // Move window
  if(event->key() == Qt::Key_W) { w->move(FRONT, 5); }
  if(event->key() == Qt::Key_A) { w->move(LEFT, 5); }
  if(event->key() == Qt::Key_D) { w->move(RIGHT, 5); }
  if(event->key() == Qt::Key_S) { w->move(BACK, 5); }

};

MainWindow::~MainWindow() { delete ui; }
