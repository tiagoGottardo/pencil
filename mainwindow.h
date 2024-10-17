#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_pBx1_clicked();
};
#endif // MAINWINDOW_H
