#include "queenant.h"


//QWidget* parent, int _x, int _y, int _cost, int _compansate,
//int _attackValue, int _blood, bool _attackFly ,bool _waterResident
QueenAnt::QueenAnt(QWidget* parent, int _x, int _y):Tower(parent,_x, _y, 0, 0, 0, 1, true, true){
    bloodBar->setVisible(false);
}

void QueenAnt::raiseUp() {
    isRaiseUp = true;
    cartoon = new QPropertyAnimation(this, "geometry");
    cartoon->setDuration(1000);
    cartoon->setStartValue(QRect(0, x_*80, 80, 80));
    cartoon->setEndValue(QRect(7*80 , x_*80, 80, 80));
    cartoon->start();
    timer = new QTimer(this);
    timer->start(30);
    hideTimer = new QTimer(this);
    hideTimer->start(1000);
    connect(timer, &QTimer::timeout, this, [=](){
        if (isAlive && isRaiseUp) {
            action();
        }
    });
    connect(hideTimer, &QTimer::timeout, this, [=](){
        this->hide();
        isAlive = false;
        hideTimer->stop();
        timer->stop();
        cartoon->stop();
    });
}

void QueenAnt::action() {
    //更新自己位置
    x_ = this->y()/80;
    y_ = this->x()/80;
}

void QueenAnt::stop() {
}

void QueenAnt::recover() {
}
