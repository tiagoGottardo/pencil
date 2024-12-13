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
  DisplayFile displayFile = DisplayFile();

protected:
  void keyPressEvent(QKeyEvent *event) override;
private slots:
  void triggerRotate();
};
