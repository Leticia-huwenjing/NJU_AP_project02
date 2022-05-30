#ifndef NINJAANT_H
#define NINJAANT_H

#include "enemy.h"
#include "tower.h"

class NinjaAnt : public Tower { //近战攻击
public:
    NinjaAnt(QWidget* parent, int _x, int _y);
    void action();
    Enemy* target;
    void stop();
    void recover();
};

#endif // NINJAANT_H
