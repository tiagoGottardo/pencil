#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Line;
class Polygon;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

    Polygon* myPolygon;
    std::vector<Line>* lines;

protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_pBx1_clicked();
    void on_pBx2_clicked();
};
#endif // MAINWINDOW_H
