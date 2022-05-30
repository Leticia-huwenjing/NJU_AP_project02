#ifndef MAPCONFIG_H
#define MAPCONFIG_H


#include "lattice.h"
#include <QString>
#include <QFile>
#include <QVector>

class MapConfig
{
public:
    MapConfig(QString path);
    QVector<QVector<Lattice*>>& getMap();
    QVector<int>& getPaths();
    int getMoney();
    int getEnemyNumber();
    int getBeesNumber();
    int getTanksNumber();
    int getFlyANumber();
    int getFlyBNumber();
private:
    QVector<QVector<Lattice*>> map;
    QVector<int> paths;
    int money = 150; //初始金钱
    int enemyNumber = 30;  //初始怪物剩余数量
    int beesNumber=enemyNumber, tankNumber=0, flyANumber=0, flyBNumber=0;
};

#endif // MAPCONFIG_H


