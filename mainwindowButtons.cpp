#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>

#include "./classes/Model.h"
#include "./classes/Factory.h"
#include "./classes/Transformable.h"

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
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->move(Point(500));
  }

  void MainWindow::on_increaseY_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->move(Point(0, 500));
  }

  void MainWindow::on_increaseZ_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->move(Point(0, 0, 500));
  }

  void MainWindow::on_decreaseX_clicked() {
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->move(Point(-500, 0, 0));
  }

  void MainWindow::on_decreaseY_clicked() {
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->move(Point(0, -500, 0));
  }

  void MainWindow::on_decreaseZ_clicked() {
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->move(Point(0, 0, -500));
  }

  void MainWindow::on_increaseXRotation_clicked() {
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->rotateX(10);
  }

  void MainWindow::on_increaseYRotation_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->rotateY(10);
  }

  void MainWindow::on_increaseZRotation_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->rotateZ(10);
  }

  void MainWindow::on_decreaseXRotation_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->rotateX(-10);
  }

  void MainWindow::on_decreaseYRotation_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->rotateY(-10);
  }

  void MainWindow::on_decreaseZRotation_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->rotateZ(-10);
  }

  void MainWindow::on_increaseScale_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->scale(.1);
  }

  void MainWindow::on_decreaseScale_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->scale(-.1);
  }

  void MainWindow::on_toggleFixedXRotation_clicked() {
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->fixedXRotation = !transformable->fixedXRotation;
  }

  void MainWindow::on_toggleFixedYRotation_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->fixedYRotation = !transformable->fixedYRotation;
  }

  void MainWindow::on_toggleFixedZRotation_clicked() { 
    Transformable* transformable = dynamic_cast<Transformable*>(displayFile[displayFileIndex].get());
    transformable->fixedZRotation = !transformable->fixedZRotation;
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
