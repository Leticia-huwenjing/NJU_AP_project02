#include "enemy.h"

Enemy::Enemy(QWidget* parent, int time, int _x, int _y) {
    setScaledContents(true);
    setParent(parent);
    x_ = _y;
    y_ = _x;
    begin_y = y_;
    timer = new QTimer(this);
    beginTimer = new QTimer(this);
    posTimer = new QTimer(this);
    slowTimer = new QTimer(this);
    beginTimer->setInterval(time);
    beginTimer->start(time);
    resize(80, 80);
    bloodBar = new QProgressBar(this);
    bloodBar->setMinimum(0);
    bloodBar->setTextVisible(false);
    bloodBar->setFixedSize(70, 5);
    bloodBar->setStyleSheet("QProgressBar{border:1px;background:white;} QProgressBar::chunk{border-radius:5px;background:red}");
    attackTimer = new QTimer(this);
    attackTimer->setInterval(500);
    isAction = false;
}

void Enemy::die() {
    isAlive = false;
    blood = -1;
    hide();
    posTimer->stop();
    timer->stop();
    slowTimer->stop();
}

void Enemy::action() {
}

