#ifndef SLOWANT_H
#define SLOWANT_H

#include "enemy.h"
#include "tower.h"

class SlowAnt : public Tower { //减速，远程
public:
    SlowAnt(QWidget* parent, int _x, int _y);
    void action();
    Enemy* target;
    void stop();
    void recover();
};

#endif // SLOWANT_H
