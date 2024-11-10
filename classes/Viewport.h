#include <QFrame>
#include <QPainter>

class Viewport : public QFrame {
  // Q_OBJECT
  
public:
  void setDisplayFile(std::vector<Drawable*> *displayFile) {
    this->displayFile = displayFile;
  }

  void setDisplayIndex(int newIndex) {
    this->displayIndex = newIndex;
  }

  explicit Viewport(QWidget *parent = nullptr) : QFrame(parent) {}

protected:
  void paintEvent(QPaintEvent *event) override {
    event->ignore();
    QPainter painter(this);
    QPen pen(Qt::black);
    painter.setPen(pen);

    for(int i = 0; i < (int) (*displayFile).size(); i++){
      if(i == displayIndex) {
        painter.setPen(Qt::red);
        (*displayFile)[i]->draw(&painter);
        painter.setPen(Qt::black);
      } else (*displayFile)[i]->draw(&painter);
    }
  }

private:
  int displayIndex;
  std::vector<Drawable*> *displayFile;
};
