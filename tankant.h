#ifndef TANKANT_H
#define TANKANT_H

#include "enemy.h"
#include "tower.h"

class TankAnt : public Enemy { //近程攻击的敌方地面单位
public:
    TankAnt(QWidget* parent, int time, int _x, int _y);
    void action();
    void stop();
    void recover();
    Tower* target;
    QPropertyAnimation* cartoon;
};

#endif // TANKANT_H
