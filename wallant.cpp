#include "wallant.h"

/*Tower(QWidget* parent, int _x, int _y, int _cost, int _compansate,
      int _attackValue, int _blood, bool _attackFly ,bool _waterResident);
      */

//远程攻击
WallAnt::WallAnt(QWidget* parent, int _x, int _y)
    :Tower(parent, _x, _y, 80, 40, 0, 100, false, false){
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    scopeX = 0;
    scopeY = 0;
    type=4;
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, [=](){
        if (isAlive) {
            action();
        }
    });
}

void WallAnt::action() {
    //更新bloodBar
    bloodBar->setValue(blood);
}

void WallAnt::stop() {
    if (isAlive) {
        timer->stop();
    }
}

void WallAnt::recover() {
    if (isAlive) {
        timer->start();
    }
}
