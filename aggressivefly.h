#ifndef AGGRESSIVEFLY_H
#define AGGRESSIVEFLY_H

#include "enemy.h"
#include "tower.h"

class AggressiveFly : public Enemy { //恋战飞机
public:
    AggressiveFly(QWidget* parent, int time, int _x, int _y);
    void action();
    void stop();
    void recover();
    Tower* target;
    QPropertyAnimation* cartoon;
};

#endif // AGGRESSIVEFLY_H
