#include "harvesterant.h"


/*Tower(QWidget* parent, int _x, int _y, int _cost, int _compansate,
      int _attackValue, int _blood, bool _attackFly ,bool _waterResident);
      */

HarvesterAnt::HarvesterAnt(QWidget* parent, int _x, int _y)
    :Tower(parent, _x, _y, 50, 30, 0, 30, false, false){
    bloodBar->setMaximum(maxBlood);
    bloodBar->setValue(maxBlood);
    scopeX = 0;
    scopeY = 0;
    type=1;
    sunPos=1;
    curSun = nullptr;
    timer = new QTimer(this);
    timer->setInterval(500);
    timer->start(500);
    moneyTimer = new QTimer(this);
    moneyTimer->setInterval(8000);
    moneyTimer->start(8000);
    posButton = new QPushButton(this);
    posButton->resize(30, 30);
    posButton->move(50, 50);
    posButton->setText("上");
    posButton->hide();
    connect(posButton, &QPushButton::clicked, this, [=](){
        //产出阳光的位置，1上，2下，3左，4右
        switch (sunPos) {
            case 1 : {
                sunPos = 2;
                posButton->setText("下");
                break;
            }
            case 2 : {
                sunPos = 3;
                posButton->setText("左");
                break;
            }
            case 3 : {
                sunPos = 4;
                posButton->setText("右");
                break;
            }
            case 4 : {
                sunPos = 1;
                posButton->setText("上");
                break;
            }
        }
    });
    connect(timer, &QTimer::timeout, this, [=](){
        if (isAlive) {
            action();
        }
    });
    connect(moneyTimer, &QTimer::timeout, this, [=](){
        moneyTimer->setInterval(8000);
        if (isAlive && !haveMoney) {
            haveMoney = true;
            curSun = new Sun(parent);
            curSun->timer->stop();
            //产出阳光的位置，1上，2下，3左，4右
            switch (sunPos) {
                case 1 : {
                    curSun->move(this->x(), this->y()-60);
                    curSun->show();
                    break;
                }
                case 2 : {
                    curSun->move(this->x(), this->y()+60);
                    curSun->show();
                    break;
                }
                case 3 : {
                    curSun->move(this->x()-60, this->y());
                    curSun->show();
                    break;
                }
                case 4 : {
                    curSun->move(this->x()+60, this->y());
                    curSun->show();
                    break;
                }
            }
        }
    });
}



void HarvesterAnt::action() {
    //更新bloodBar
    bloodBar->setValue(blood);
}

void HarvesterAnt::stop() {
    if (isAlive) {
        timer->stop();
        int moneyTimerRemaining = moneyTimer->remainingTime();
        moneyTimer->stop();
        moneyTimer->setInterval(moneyTimerRemaining);
    }
}

void HarvesterAnt::recover() {
    if (isAlive) {
        timer->start();
        moneyTimer->start();
    }
}

void HarvesterAnt::mousePressEvent(QMouseEvent* ev){
    if (isAlive){
        if (ev->button() == Qt::LeftButton) {
            if (!detail) { //展示详情
                qDebug() << "鼠标点击了(" << x_ << "," << y_ << ")";
                emit chooseTower();
                detail = true;
                bloodBar->setVisible(true);
                posButton->show();
            } else {
                detail = false;
                bloodBar->setVisible(false);
                posButton->hide();
            }
        }
    }
}
