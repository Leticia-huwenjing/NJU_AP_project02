#include "tower.h"

int Tower::getType(){
    return type;
}

int Tower::getCost() {
    return cost;
}

int Tower::getCompensate() {
    return compansate;
}

Tower::Tower(QWidget* parent, int _x, int _y, int _cost, int _compansate,
         int _attackValue, int _blood, bool _attackFly ,bool _waterResident)
    :QLabel(parent){
    x_ = _x;
    y_ = _y;
    cost = _cost;
    bloodBar = new QProgressBar(this);
    bloodBar->setMinimum(0);
    bloodBar->setTextVisible(false);
    bloodBar->setFixedSize(70, 5);
    bloodBar->setVisible(false);
    bloodBar->setStyleSheet("QProgressBar{border:1px;background:white;} QProgressBar::chunk{border-radius:5px;background:red}");
    compansate = _compansate;
    attackValue = _attackValue;
    blood = _blood;
    maxBlood = blood;
    attackFly = _attackFly;
    waterResident = _waterResident;
    attackTimer = new QTimer(this);
    attackTimer->setInterval(500);
    isAction = false;
}

void Tower::die() {
    this->hide();
    timer->stop();
    isAlive = false;
}

void Tower::mousePressEvent(QMouseEvent* ev){
    if (isAlive){
        if (ev->button() == Qt::LeftButton) {
            if (!detail) { //展示详情
                qDebug() << "鼠标点击了(" << x_ << "," << y_ << ")";
                emit chooseTower();
                detail = true;
                bloodBar->setVisible(true);
            } else {
                detail = false;
                bloodBar->setVisible(false);
            }
        }
    }
}




















