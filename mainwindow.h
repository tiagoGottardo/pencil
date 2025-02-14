#pragma once

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
  DisplayFile displayFile;

protected:
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void triggerRotate();

  void on_left_clicked();
  void on_right_clicked();

  void on_increaseX_clicked();
  void on_increaseY_clicked();
  void on_increaseZ_clicked();

  void on_decreaseX_clicked();
  void on_decreaseY_clicked();
  void on_decreaseZ_clicked();

  void on_increaseXRotation_clicked();
  void on_increaseYRotation_clicked();
  void on_increaseZRotation_clicked();

  void on_decreaseXRotation_clicked();
  void on_decreaseYRotation_clicked();
  void on_decreaseZRotation_clicked();

  void on_increaseScale_clicked();
  void on_decreaseScale_clicked();

  void on_toggleFixedXRotation_clicked();
  void on_toggleFixedYRotation_clicked();
  void on_toggleFixedZRotation_clicked();

  void on_addDonut_clicked();
  void on_addCharizard_clicked();
  void on_addPsyduck_clicked();

  void on_deleteObject_clicked();
  void on_importObject_clicked();
};
