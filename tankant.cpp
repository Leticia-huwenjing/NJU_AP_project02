#include "tankant.h"

TankAnt::TankAnt(QWidget* parent, int time, int _x, int _y)
    : Enemy(parent, time, _x, _y){
    blood = 50;
    maxBlood = blood;
    target = nullptr;
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    bloodBar->setVisible(false);
    connect(beginTimer, &QTimer::timeout, this, [=](){
        this->show();
        isAlive = true;
        isShowed = true;
        timer->setInterval(2000);
        timer->start(2000);
        posTimer->setInterval(30);
        posTimer->start(30);
        cartoon = new QPropertyAnimation(this, "geometry");
        cartoon->setDuration(15000);
        cartoon->setStartValue(QRect(_x*80, _y*80, 80, 80));
        cartoon->setEndValue(QRect(0, _y*80, 80, 80));
        cartoon->start();
        bloodBar->setVisible(true);
        bloodBar->show();
        beginTimer->stop();
    });
    connect(timer, &QTimer::timeout, this, [=](){
        if(isAlive) {
            action();
        }
    });
    connect(posTimer, &QTimer::timeout, this, [=](){
        if (isAlive) {
            //更新x,y矩阵坐标
            x_ = this->y()/80;
            y_ = this->x()/80;

            //更新slowTimer
            if (isSlowed && !slowTimer->isActive()) {
                slowTimer->setInterval(2000);
                slowTimer->start(2000);
                cartoon->stop();
                cartoon->setStartValue(QRect(this->x(), this->y(), 80, 80));
                cartoon->setDuration(45000*((double)this->x())/(begin_y*80));
                cartoon->setEndValue(QRect(0, this->y(), 80, 80));
                cartoon->start();
            }

            //被阻拦
            if (target != nullptr) {
                if (target->getType() == 6 || target->getType() == 4 || target->getType() == 7 || target->getType() == 3) {
                    cartoon->pause();
                } else {
                    cartoon->start();
                }
            } else {
                cartoon->start();
            }
        }
    });

    connect(slowTimer, &QTimer::timeout, this, [=](){
        isSlowed = false;
        slowTimer->stop();
        cartoon->stop();
        cartoon->setStartValue(QRect(this->x(), this->y(), 80, 80));
        cartoon->setDuration(15000*((double)this->x())/(begin_y*80));
        cartoon->setEndValue(QRect(0, this->y(), 80, 80));
        cartoon->start();
    });


}

void TankAnt::action() {
    //更新bloodBar
    bloodBar->setValue(blood);

    //攻击防御塔
    if (target != nullptr) {
        target->blood -= 15;
        AttackLabel* attack = new AttackLabel(target);
        attack->setText("-15");
        attack->setStyleSheet("color:rgba(255, 0, 0, 0.7);");
        attack->show();
        QPropertyAnimation* attackCartoon = new QPropertyAnimation(attack, "geometry");
        attackCartoon->setDuration(1000);
        attackCartoon->setStartValue(QRect(0, 0, attack->width(), attack->height()));
        attackCartoon->setEndValue(QRect(80, 0, attack->width(), attack->height()));
        attackCartoon->start();
    }
}

void TankAnt::stop() {
    if (!isShowed) {
        beginTimer->stop();
    }
    if (isAlive) {
        cartoon->pause();
        posTimer->stop();
        timer->stop();
        if (isSlowed) {
            slowTimer->stop();
        }
    }
}

void TankAnt::recover() {
    if (!isShowed) {
        beginTimer->start();
    }
    if (isAlive) {
        timer->start();
        posTimer->start();
        cartoon->start();
        if (isSlowed) {
            slowTimer->start();
        }
    }
}
