#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

class Point {
public:
    int32_t x;
    int32_t y;
    int32_t z;

    std::string name = "Dot";

    Point(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {};

    std::string self(){
        std::stringstream ss;
        ss << name << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }

    void checkItself(){
        std::cout << self() << std::endl;
    };

    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

class Line {
public:
    Point* a;
    Point* b;

    std::string name = "Line";

    Line(Point* a, Point* b) : a(a), b(b) {};

    std::string self() {
        std::stringstream ss;
        ss << name << "|" << a->self() << " -> " << b->self() << "|";
        return ss.str();
    };

    void checkItself(){
        std::cout << self() << std::endl;
    };

    void draw(QPainter* painter){
        painter->drawLine(a->x, a->y, b->x, b->y);
    }

    double calculateAngular(){
        int32_t dy = b->y - a->y;
        int32_t dx = b->x - a->x;

        if(dx == 0) {
            return std::numeric_limits<double>::infinity();
        }

        return dy/dx;
    }
};

class Polygon {
public:
    std::vector<Line>* lines;
    Point ref = Point(0, 0, 0);

    std::string name = "Polygon";

    Polygon(std::vector<Line>* linesList) {
        if(linesList->size() <= 2) {
            printf("You need 3 dots or more to create a polygon.\n");
            return;
        }

        bool couldBePolygon = true;
        for(uint i = 0; i < linesList->size(); i++){
            if(i + 1 == linesList->size()){
                if((*linesList)[i].b != (*linesList)[0].a){
                    couldBePolygon = false;
                }
                if((*linesList)[i].calculateAngular() == (*linesList)[i + 1].calculateAngular()){
                    printf("There are two lines in place of one.\n");
                    couldBePolygon = false;
                }
                break;
            }

            if((*linesList)[i].b != (*linesList)[i + 1].a){
                couldBePolygon = false;
                break;
            }

            if((*linesList)[i].calculateAngular() == (*linesList)[i + 1].calculateAngular()){
                printf("There are two lines in place of one.\n");
                break;
            }
        }

        if(!couldBePolygon) {
            printf("The lines doesn't makes a possible polygon.\n");
            return;
        }

        lines = linesList;
    };

    void checkItself() {
        printf("Polygon {\n");
        for(uint i = 0; i < lines->size(); i++){
            (*lines)[i].checkItself();
        }
        printf("}\n");
    };

    Point calculateCentroid(){
        int Cx = 0, Cy = 0;
        std::vector<Point*> dots = getDots();

        uint size = dots.size();

        if (size == 0){
            throw std::invalid_argument("There is no dots in centroid calculation.");
        };

        for(uint i = 0; i < size; i++){
            Cx += dots[i]->x;
            Cy += dots[i]->y;
        }

        return Point(Cx/size, Cy/size, 0);
    };

    std::vector<Point*> getDots(){
        std::vector<Point*> dots;

        for(uint i = 0; i < (*lines).size(); i++){
            dots.push_back((*lines)[i].a);
        }

        return dots;
    };

    void redimensionXY(float s){
        std::vector<Point*> dots = getDots();

        Point centroid = calculateCentroid();

        for(uint i = 0; i < dots.size(); i++){
            dots[i]->y = centroid.y + s * (dots[i]->y - centroid.y);
            dots[i]->x = centroid.x + s * (dots[i]->x - centroid.x);
        }
    };

    void draw(QPainter* painter){
        for(uint i = 0; i < (*lines).size(); i++) {
            (*lines)[i].draw(painter);
        }
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::vector<Point> points;
    lines = new std::vector<Line>();

    Point* a = new Point(50, 50, 0);
    Point* b = new Point(100, 200, 0);
    Point* c = new Point(300, 300, 0);

    Line A(a, b);
    Line B(b, c);
    Line C(c, a);

    lines->push_back(A);
    lines->push_back(B);
    lines->push_back(C);

    myPolygon = new Polygon(lines);

    myPolygon->checkItself();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Set pen color and width
    QPen pen(Qt::white, 3);
    pen.setWidth(1);
    painter.setPen(pen);


    myPolygon->draw(&painter);
}

void MainWindow::on_pBx1_clicked() {
    myPolygon->redimensionXY(1.5);
    myPolygon->checkItself();
    update();
}

void MainWindow::on_pBx2_clicked() {
    myPolygon->redimensionXY(0.6666666666);
    myPolygon->checkItself();
    update();
}
