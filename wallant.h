#ifndef WALLANT_H
#define WALLANT_H

#include "enemy.h"
#include "tower.h"


class WallAnt : public Tower { //坚果墙，近战
public:
    WallAnt(QWidget* parent, int _x, int _y);
    void action();
    void stop();
    void recover();
};

#endif // WALLANT_H
