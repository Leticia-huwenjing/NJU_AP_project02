#include "fireant.h"

/*Tower(QWidget* parent, int _x, int _y, int _cost, int _compansate,
      int _attackValue, int _blood, bool _attackFly ,bool _waterResident);
      */

//远程攻击
FireAnt::FireAnt(QWidget* parent, int _x, int _y)
    :Tower(parent, _x, _y, 350, 10, 70, 10000, true, false){
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    scopeX = 0;
    scopeY = 0;
    type=8;
    timer = new QTimer(this);
    timer->setInterval(3000);
    timer->start(3000); //3秒后爆炸
    connect(timer, &QTimer::timeout, this, [=](){
        if (isAlive) {
            action();
        }
    });
}

void FireAnt::action() {
    blood = 0; //调用一次就死亡
    for (auto& enemy : targets) {
        if (enemy != nullptr && enemy->isAlive) {
            AttackLabel* attack = new AttackLabel(enemy);
            attack->setText("-70");
            attack->setStyleSheet("color:rgba(255, 0, 0, 0.7);");
            attack->show();
            QPropertyAnimation* attackCartoon = new QPropertyAnimation(attack, "geometry");
            attackCartoon->setDuration(1000);
            attackCartoon->setStartValue(QRect(0, 0, attack->width(), attack->height()));
            attackCartoon->setEndValue(QRect(80, 0, attack->width(), attack->height()));
            attackCartoon->start();
            enemy->blood -= 70;
        }
    }
    //产生爆炸特效
    QTimer* boomTimer = new QTimer(this);
    boomTimer->start(1400);
    QLabel* boomLabel = new QLabel(this->parentWidget());
    boomLabel->move(this->x()-80, this->y()-80);
    boomLabel->resize(80*3, 80*3);
    boomLabel->setScaledContents(true);
    QMovie* mov = new QMovie(":/boom.gif");
    boomLabel->setMovie(mov);
    mov->start();
    boomLabel->show();
    connect(boomTimer, &QTimer::timeout, this, [=](){
        boomTimer->stop();
        boomLabel->hide();
    });
    timer->stop();
    this->hide();
}

void FireAnt::stop() {
    if (isAlive) {
        interval = timer->remainingTime();
        timer->stop();
        timer->setInterval(interval);
    }
}

void FireAnt::recover() {
    if (isAlive) {
        timer->start();
    }
}
