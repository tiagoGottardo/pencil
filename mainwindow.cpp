#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include <vector>
#include <limits>

class Point {
public:
    uint32_t x;
    uint32_t y;

    Point(double x, double y) : x(x), y(y){};

    void checkItself(){
        printf("(%d, %d)\n", x, y);
    };

    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);  // Use the == operator to define !=
    }
};

class Line {
public:
    Point a;
    Point b;

    Line(Point a, Point b) : a(a), b(b){};

    void checkItself(){
        printf("From: (%d, %d)  ", a.x, a.y);
        printf("To: (%d, %d)\n", b.x, b.y);
    };

    void draw(QPainter* painter){
        painter->drawLine(a.x, a.y, b.x, b.y);
    }

    double calculateAngular(){
        int dy = b.y - a.y;
        int dx = b.x - a.x;

        if(dx == 0) {
            return std::numeric_limits<double>::infinity();
        }

        return dy/dx;
    }
};

class Polygon {
public:
    std::vector<Line> lines;
    Point ref = Point(0, 0);

    Polygon(std::vector<Line> linesList) {
        if(linesList.size() <= 2) {
            printf("You need 3 dots or more to create a polygon.\n");
            return;
        }

        bool couldBePolygon = true;
        for(uint i = 0; i < linesList.size(); i++){
            if(i + 1 == linesList.size()){
                if(linesList[i].b != linesList[0].a){
                    couldBePolygon = false;
                }
                if(linesList[i].calculateAngular() == linesList[i + 1].calculateAngular()){
                    printf("There are two lines in place of one.\n");
                    couldBePolygon = false;
                }
                break;

            }

            if(linesList[i].b != linesList[i + 1].a){
                couldBePolygon = false;
                break;
            }

            if(linesList[i].calculateAngular() == linesList[i + 1].calculateAngular()){
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
        for(uint i = 0; i < lines.size(); i++){
            lines[i].checkItself();
        }
    };

    Point calculateCentroid(){
        int Cx = 0, Cy = 0;
        std::vector<Point> *dots = getDots();

        uint size = dots->size();

        if (size == 0){
            throw std::invalid_argument("There is no dots in centroid calculation.");
        };

        for(uint i = 0; i < size; i++){
            Cx += (*dots)[i].x;
            Cy += (*dots)[i].y;
        }

        return Point(Cx/size, Cy/size);
    };

    std::vector<Point>* getDots(){
        std::vector<Point>*dots = new std::vector<Point>;

        for(uint i = 0; i < lines.size(); i++){
            dots->push_back(lines[i].a);
        }
        dots->push_back(lines.back().b);

        return dots;
    };

    void redimensionX(float s){
        std::vector<Point>* dots = getDots();

        Point centroid = calculateCentroid();

        for(uint i = 0; i < dots->size(); i++){
            (*dots)[i].x = centroid.x + s * ((*dots)[i].x - centroid.x);
        }
    };

    //void redimensionY(float s){
    //    std::vector<Point> dots = getDots();

    //    Point centroid = calculateCentroid();

    //    for(uint i = 0; i < dots.size(); i++){
    //        dots[i].y = centroid.y + s * (dots[i].y - centroid.y);
    //    }
    //};

    //void redimensionXY(float s){
    //    redimensionX(s);
    //    redimensionY(s);
    //};

    void draw(QPainter* painter){
        for(uint i = 0; i < lines.size(); i++) {
            lines[i].draw(painter);
        }
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::vector<Point> points;
    std::vector<Line> lines;

    Point a(50, 50);
    Point b(100, 200);
    Point c(300, 300);

    Line A(a, b);
    Line B(b, c);
    Line C(c, a);

    lines.push_back(A);
    lines.push_back(B);
    lines.push_back(C);

    myPolygon = new Polygon(lines);
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
    myPolygon->redimensionX(1.5);
    myPolygon->checkItself();
}
