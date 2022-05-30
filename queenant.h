#ifndef QUEENANT_H
#define QUEENANT_H

#include "tower.h"
#include "enemy.h"
#include <QPropertyAnimation>

class QueenAnt : public Tower {
public:
    QueenAnt(QWidget* parent, int _x, int _y);
    void raiseUp(); //被激活
    bool isRaiseUp = false;
    void action();
    QTimer* hideTimer;
    QPropertyAnimation* cartoon;
    void stop();
    void recover();
};

#endif // QUEENANT_H
