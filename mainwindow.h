#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./classes/Drawable.h"
#include "./classes/Viewport.h"

#include <QCoreApplication>
#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE;
namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE;

class MainWindow : public QMainWindow {
  Q_OBJECT;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  Viewport viewport;
  int displayFileIndex;
  vector<Drawable*> displayFile = vector<Drawable*>();

protected:
  void keyPressEvent(QKeyEvent *event) override;
private slots:
  void on_left_clicked();
  void on_right_clicked();

  void on_moveRight_clicked();
  void on_moveLeft_clicked();
  void on_moveTop_clicked();
  void on_moveBottom_clicked();

  void on_upScale_clicked();
  void on_downScale_clicked();

  void on_clockRotation_clicked();
  void on_antiClockRotation_clicked();

  void on_createPolygon_clicked();
};
#endif
