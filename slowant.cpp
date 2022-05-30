#include "slowant.h"

/*Tower(QWidget* parent, int _x, int _y, int _cost, int _compansate,
      int _attackValue, int _blood, bool _attackFly ,bool _waterResident);
      */

//远程攻击
SlowAnt::SlowAnt(QWidget* parent, int _x, int _y)
    :Tower(parent, _x, _y, 100, 50, 5, 30, true, false){
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    scopeX = 0;
    scopeY = 0;
    type=5;
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

void SlowAnt::action() {
    //更新bloodBar
    bloodBar->setValue(blood);

    //攻击敌人
    if (target != nullptr) {
        target->blood -= 5;
        target->isSlowed = true;
        AttackLabel* attack = new AttackLabel(target);
        attack->setText("-5");
        attack->setStyleSheet("color:rgba(255, 0, 0, 0.7);");
        attack->show();
        QPropertyAnimation* attackCartoon = new QPropertyAnimation(attack, "geometry");
        attackCartoon->setDuration(1000);
        attackCartoon->setStartValue(QRect(0, 0, attack->width(), attack->height()));
        attackCartoon->setEndValue(QRect(80, 0, attack->width(), attack->height()));
        attackCartoon->start();

        //控制攻击特效
        attackTimer->start();
        isAction = true;
        connect(attackTimer, &QTimer::timeout, this, [=](){
            isAction = false;
            attackTimer->stop();
        });
    }
}

void SlowAnt::stop() {
    if (isAlive) {
        timer->stop();
    }
}

void SlowAnt::recover() {
    if (isAlive) {
        timer->start();
    }
}
