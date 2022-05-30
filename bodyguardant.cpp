#include "bodyguardant.h"


/*Tower(QWidget* parent, int _x, int _y, int _cost, int _compansate,
      int _attackValue, int _blood, bool _attackFly ,bool _waterResident);
      */

BodyguardAnt::BodyguardAnt(QWidget* parent, int _x, int _y)
    :Tower(parent, _x, _y, 30, 10, 0, 50, false, false){
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    scopeX = 0;
    scopeY = 0;
    type=7;
    timer = new QTimer(this);
    timer->setInterval(500);
    timer->start(500);
    connect(timer, &QTimer::timeout, this, [=](){
        if (isAlive) {
            action();
        }
    });
}

void BodyguardAnt::action() {
    //更新bloodBar
    bloodBar->setValue(blood);
}

void BodyguardAnt::stop() {
    if (isAlive) {
        timer->stop();
    }
}

void BodyguardAnt::recover() {
    if (isAlive) {
        timer->start();
    }
}
