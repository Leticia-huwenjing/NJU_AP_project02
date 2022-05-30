
#ifndef HARVESTERANT_H
#define HARVESTERANT_H

#include "tower.h"
#include "enemy.h"
#include "sun.h"
#include <QPushButton>



class HarvesterAnt : public Tower { //增加金钱
public:
    HarvesterAnt(QWidget* parent, int _x, int _y);
    void mousePressEvent(QMouseEvent* ev);
    void action(); 
    void stop();
    void recover();

    QTimer* moneyTimer;
    Sun* curSun;
    QPushButton* posButton;
    bool haveMoney = false; //是否产出了金钱
    int sunPos=1; //产出阳光的位置，1上，2下，3左，4右
};

#endif // HARVESTERANT_H
