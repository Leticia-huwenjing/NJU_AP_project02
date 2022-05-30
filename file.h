#ifndef FILE_H
#define FILE_H

#include "lattice.h"
#include <QVector>
#include <QString>

class File {
public:
    File(QString path);
    QVector<QVector<Lattice*>>& getMap();
    QVector<int>& getPaths();
    int getMoney();
    int getLives();
    int getEnemyNumber();
    int getBeesNumber();
    int getTanksNumber();
    int getFlyANumber();
    int getFlyBNumber();
private:
    QVector<QVector<Lattice*>> map;
    QVector<int> paths;
    int money = 150; //初始金钱
    int lives = 10; //初始生命值
    int enemyNumber = 30;  //初始怪物剩余数量
    int beesNumber=enemyNumber, tankNumber=0, flyANumber=0, flyBNumber=0;
};

#endif // FILE_H
