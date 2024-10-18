#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "classes/Point.h"
#include "classes/Line.h"
#include "classes/Polygon.h"

#include <QCoreApplication>
#include <QMainWindow>
#include <QKeyEvent>

#include<variant>
#include <memory>
#include <vector>

// QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
}
// QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  std::vector<Thing*>* displayFile;
  // std::unique_ptr<std::vector<std::variant<Point, Line, Polygon>>> displayFile;
  // std::vector<std::variant<Point, Line, Polygon>>* displayFile;

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override {
    if(event->key() == Qt::Key_Return) {
      QCoreApplication::quit();
    }
  };

private slots:
  void on_pBx1_clicked();
  void on_pBx2_clicked();

};
#endif
