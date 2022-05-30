#include "sun.h"

Sun::Sun(QWidget* parent) : QLabel(parent) {
    isClicked = false;
    iscnt = false;
    this->resize(60, 60);
    this->setScaledContents(true);
    this->setPixmap(QPixmap(":/sun.png"));
    timer = new QTimer(this);
    timer->start(4000);
    connect(timer, &QTimer::timeout, this, [=](){
        this->hide();
    });
}

void Sun::mousePressEvent(QMouseEvent *ev) {
    isClicked = true;
    this->hide();
}
