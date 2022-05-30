#ifndef BEE_H
#define BEE_H

#include "enemy.h"
#include "tower.h"

class Bee : public Enemy { //远程攻击的地面单位
public:
    Bee(QWidget* parent, int time, int _x, int _y);
    void action();
    void stop();
    void recover();
    Tower* target;
    QPropertyAnimation* cartoon;
};

#endif // BEE_H
