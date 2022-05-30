#ifndef STATE_H
#define STATE_H

//控制全局状态，游戏主窗口

#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QWidget>
#include <QRandomGenerator>

#include "mapconfig.h"
#include "lattice.h"

#include "tower.h"
#include "harvesterant.h"
#include "scubaant.h"
#include "throwerant.h"
#include "queenant.h"
#include "wallant.h"
#include "slowant.h"
#include "ninjaant.h"
#include "bodyguardant.h"
#include "fireant.h"


#include "enemy.h"
#include "bee.h"
#include "tankant.h"
#include "aggressivefly.h"
#include "scaryfly.h"

#include "sun.h"


#include <QPainter>
#include <QPixmap>
#include <QMovie>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>

#define SIZE 80
#define W 80
#define H 40

class State : public QWidget {
    Q_OBJECT
public:
    State(QString path); //在构造函数中完成全部的初始化
    void play();
    void updateStatus();
        void connectTower();

signals:
    void wantAdd();

private:
    QVector<QVector<Lattice*>> map;
    QVector<int> paths;

    QVector<Bee*> Bees;
    QVector<TankAnt*> TankAnts;
    QVector<AggressiveFly*> AggressiveFlys;
    QVector<ScaryFly*> ScaryFlys;

    QVector<QueenAnt*> QueenAnts; 
    QVector<HarvesterAnt*> HarvesterAnts;
    QVector<ThrowerAnt*> ThrowerAnts;
    QVector<ScubaAnt*> ScubaAnts;
    QVector<WallAnt*> WallAnts;
    QVector<SlowAnt*> SlowAnts;
    QVector<NinjaAnt*> NinjaAnts;
    QVector<BodyguardAnt*> BodyguardAnts;
    QVector<FireAnt*> FireAnts;

    QVector<Sun*> Suns;

    QTimer *timer; //游戏总控制
    QTimer *drawTimer; //绘图控制，不受暂停影响
    QTimer* sunTimer; //模拟随机掉落阳光,受暂停影响

    int money = 150; //初始金钱
    int enemyNumber = 30;  //初始怪物剩余数量
    int beesNumber=enemyNumber, tankNumber=0, flyANumber=0, flyBNumber=0;
    bool gameOver = false; //游戏输掉flag
    bool win = false; //游戏胜利flag
    int pressMoney = 10; //剩余掉落阳光数量


    void paintEvent(QPaintEvent *event); //绘图事件,调用update重新绘制
    void drawMap(QPainter& painter); //绘制地图
    void drawTowerScope(QPainter& painter); //绘制攻击范围
    void drawAttack(QPainter& painter); //绘制攻击特效


    void init();
    void interfaceInit();
    void beesInit();

    void throwMoney();

    void connectButtons();

    void stop();
    void recover();

    void getPutPos(int _x, int _y); //得到想要放置东西的位置
    int x=0, y=0; //当前处理全局的位置，矩阵坐标
    Tower* pTower=nullptr; //当前处理的塔

    bool addTower(Tower* curTower); //加防御塔
    bool removeTower(Tower* curTower); //取消部署防御塔

};

#endif // STATE_H


