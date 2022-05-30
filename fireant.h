#ifndef FIREANT_H
#define FIREANT_H

#include "enemy.h"
#include "tower.h"

class FireAnt : public Tower { //樱桃炸弹
public:
    FireAnt(QWidget* parent, int _x, int _y);
    void action();
    QVector<Enemy*> targets;
    void stop();
    void recover();
    int interval=0;
};

#endif // FIREANT_H
