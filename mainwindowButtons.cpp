#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>

#include "./classes/Model.h"
#include "./classes/Factory.h"

void MainWindow::on_left_clicked() { if(displayFile.size() == 0) return;
  if(displayFile.size() == 0) return;

  if(displayFileIndex == 0) displayFileIndex = displayFile.size() - 1;
  else displayFileIndex--;
}

void MainWindow::on_right_clicked() {
  if(displayFile.size() == 0) return;

  if(displayFileIndex == (int) displayFile.size() - 1) displayFileIndex = 0;
  else displayFileIndex++;
}

void MainWindow::on_increaseX_clicked() {
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->move(Point(500));
}

void MainWindow::on_increaseY_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->move(Point(0, 500));
}

void MainWindow::on_increaseZ_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->move(Point(0, 0, 500));
}

void MainWindow::on_decreaseX_clicked() {
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->move(Point(-500, 0, 0));
}

void MainWindow::on_decreaseY_clicked() {
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->move(Point(0, -500, 0));
}

void MainWindow::on_decreaseZ_clicked() {
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->move(Point(0, 0, -500));
}

void MainWindow::on_increaseXRotation_clicked() {
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->rotateX(10);
}

void MainWindow::on_increaseYRotation_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->rotateY(10);
}

void MainWindow::on_increaseZRotation_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->rotateZ(10);
}

void MainWindow::on_decreaseXRotation_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->rotateX(-10);
}

void MainWindow::on_decreaseYRotation_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->rotateY(-10);
}

void MainWindow::on_decreaseZRotation_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->rotateZ(-10);
}

void MainWindow::on_increaseScale_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->scale(.1);
}

void MainWindow::on_decreaseScale_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->scale(-.1);
}

void MainWindow::on_toggleFixedXRotation_clicked() {
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->fixedXRotation = !model->fixedXRotation;
}

void MainWindow::on_toggleFixedYRotation_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->fixedYRotation = !model->fixedYRotation;
}

void MainWindow::on_toggleFixedZRotation_clicked() { 
  Model* model = dynamic_cast<Model*>(displayFile[displayFileIndex].get());
  model->fixedZRotation = !model->fixedZRotation;
}

void MainWindow::on_addDonut_clicked() { 
  displayFile.push_back(make_unique<Model>(Factory::createDonut(40)));
  displayFileIndex = (int) displayFile.size() - 1;
}

void MainWindow::on_addCharizard_clicked() { 
  displayFile.push_back(make_unique<Model>(Factory::import("./assets/charizard.obj")));
  displayFileIndex = (int) displayFile.size() - 1;
}

void MainWindow::on_addPsyduck_clicked() { 
  displayFile.push_back(make_unique<Model>(Factory::import("./assets/psyduck.obj")));
  displayFileIndex = (int) displayFile.size() - 1;
}

void MainWindow::on_addBuilding_clicked() { 
  displayFile.push_back(make_unique<Model>(Factory::import("./assets/building.obj")));
  displayFileIndex = (int) displayFile.size() - 1;
}

void MainWindow::on_importObject_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
    nullptr,                           // Parent widget
    "Open File",                       // Dialog title
    QDir::homePath(),                  // Initial directory
    "OBJ Files (*.obj)" // File filters
  );
  displayFile.push_back(make_unique<Model>(Factory::import(fileName.toStdString())));
  displayFileIndex = (int) displayFile.size() - 1;
}

void MainWindow::on_deleteObject_clicked() {
  if(displayFile.size() == 0) return;

  displayFile.erase(displayFile.begin() +displayFileIndex);
  displayFileIndex = 0;
}
