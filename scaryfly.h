#ifndef SCARYFLY_H
#define SCARYFLY_H

#include "enemy.h"
#include "tower.h"


class ScaryFly : public Enemy { //畏战飞机
public:
    ScaryFly(QWidget* parent, int time, int _x, int _y);
    void action();
    void stop();
    void recover();
    Tower* target;
    QPropertyAnimation* cartoon;
};

#endif // SCARYFLY_H
