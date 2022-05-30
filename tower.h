#ifndef TOWER_H
#define TOWER_H

#include <QProgressBar>
#include <QTimer>
#include "lattice.h"
#include "attacklabel.h"

class Tower : public QLabel{
    Q_OBJECT
public:
    Tower(QWidget* parent, int _x, int _y, int _cost, int _compansate,
          int _attackValue, int _blood, bool _attackFly ,bool _waterResident);
    virtual void action()=0;
    virtual void stop()=0;
    virtual void recover()=0;
    void die();
    int getType();
    int getCost();
    int getCompensate();
    int blood;
    int maxBlood;
    bool isAlive = true;
    bool detail = false;
    int x_, y_; //矩阵坐标
    void mousePressEvent(QMouseEvent* ev) override;
    bool isAction = false; //获取当前是否在攻击

signals:
    void chooseTower();

protected:
    QProgressBar* bloodBar; //血条
    QTimer* timer;
    QTimer* attackTimer; //控制攻击特效显示时长
    int type=0;
    int cost=0;
    int scopeX=0; //前方身位格 e.g x=1，即能攻击自己的格子和自己面前的格子
    int scopeY=0; //上下身位格 e.g y=1，即能攻击上下两条格子的敌人 y=0即为只能攻击自己这一行的敌人
    int compansate = (int)cost*0.8;
    int attackValue;
    bool attackFly=false;
    bool waterResident=false;
};

















#endif // TOWER_H

