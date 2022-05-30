#ifndef BODYGUARDANT_H
#define BODYGUARDANT_H

#include "enemy.h"
#include "tower.h"

class BodyguardAnt : public Tower { //南瓜
public:
    BodyguardAnt(QWidget* parent, int _x, int _y);
    void action();
    void stop();
    void recover();
};

#endif // BODYGUARDANT_H
