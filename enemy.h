#ifndef ENEMY_H
#define ENEMY_H


#include <QLabel>
#include <QTimer>
#include <QMovie>
#include <QPropertyAnimation>
#include <QProgressBar>
#include "attacklabel.h"

class Enemy : public QLabel{
    Q_OBJECT
public:
    Enemy(QWidget* parent, int time, int _x, int _y);
    virtual void action();
    virtual void stop()=0;
    virtual void recover()=0;
    int x_, y_;
    int blood=0;
    int maxBlood = blood;
    bool isAlive = false;
    bool isSlowed = false;
    bool isShowed = false;
    bool isAction = false; //获取当前是否在攻击
    void die();

protected:
    bool isFly=false;
    int begin_y=0;
    QProgressBar* bloodBar; //血条
    QTimer* timer; //出现后行动计时
    QTimer* beginTimer; //出现时间
    QTimer* posTimer; //出现后位置更新
    QTimer* slowTimer; //控制被减速后恢复正常速度的时间
    QTimer* attackTimer; //控制攻击特效显示时长
};

#endif // ENEMY_H

