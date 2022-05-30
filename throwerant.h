#ifndef THROWERANT_H
#define THROWERANT_H

#include "tower.h"
#include "enemy.h"

class ThrowerAnt : public Tower { //远程攻击
public:
    ThrowerAnt(QWidget* parent, int _x, int _y);
    void action();
    Enemy* target;
    void stop();
    void recover();
};

#endif // THROWERANT_H
