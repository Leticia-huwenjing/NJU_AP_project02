#ifndef SCUBAANT_H
#define SCUBAANT_H

#include "enemy.h"
#include "tower.h"

class ScubaAnt : public Tower { //水上蚂蚁，远程(只能放在水上)
public:
    ScubaAnt(QWidget* parent, int _x, int _y);
    void action();
    Enemy* target;
    void stop();
    void recover();
};

#endif // SCUBAANT_H
