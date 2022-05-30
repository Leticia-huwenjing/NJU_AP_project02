#include "ninjaant.h"

NinjaAnt::NinjaAnt(QWidget* parent, int _x, int _y)
    :Tower(parent, _x, _y, 50, 30, 10, 50, false, false){
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    scopeX = 0;
    scopeY = 0;
    type=6;
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start(1000);
    target = nullptr;
    connect(timer, &QTimer::timeout, this, [=](){
        if (isAlive) {
            action();
        }
    });
}

void NinjaAnt::action() {
    //更新bloodBar
    bloodBar->setValue(blood);

    //攻击敌人
    if (target != nullptr) {
        target->blood -= 10;
        AttackLabel* attack = new AttackLabel(target);
        attack->setText("-10");
        attack->setStyleSheet("color:rgba(255, 0, 0, 0.7);");
        attack->show();
        QPropertyAnimation* attackCartoon = new QPropertyAnimation(attack, "geometry");
        attackCartoon->setDuration(1000);
        attackCartoon->setStartValue(QRect(0, 0, attack->width(), attack->height()));
        attackCartoon->setEndValue(QRect(80, 0, attack->width(), attack->height()));
        attackCartoon->start();
    }
}

void NinjaAnt::stop() {
    if (isAlive) {
        timer->stop();
    }
}

void NinjaAnt::recover() {
    if (isAlive) {
        timer->start();
    }
}
