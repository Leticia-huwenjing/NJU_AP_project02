#include "aggressivefly.h"

AggressiveFly::AggressiveFly(QWidget* parent, int time, int _x, int _y)
    : Enemy(parent, time, _x, _y){
    blood = 10;
    maxBlood = blood;
    target = nullptr;
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    bloodBar->setVisible(false);
    connect(beginTimer, &QTimer::timeout, this, [=](){
        this->show();
        isAlive = true;
        isShowed = true;
        timer->setInterval(300);
        timer->start(300);
        posTimer->setInterval(30);
        posTimer->start(30);
        cartoon = new QPropertyAnimation(this, "geometry");
        cartoon->setDuration(5000);
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
        //更新x,y矩阵坐标
        x_ = this->y()/80;
        y_ = this->x()/80;
    });
}

void AggressiveFly::action() {
    //更新bloodBar
    bloodBar->setValue(blood);


    //攻击防御塔
    if (target != nullptr) {
        target->blood -= 5;
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

        if (target->getType() == 6 || target->getType() == 4 || target->getType() == 7 || target->getType() == 3) {
            cartoon->pause();
        } else {
            cartoon->start();
        }
    } else {
        cartoon->start();
    }
}

void AggressiveFly::stop() {
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

void AggressiveFly::recover() {
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
