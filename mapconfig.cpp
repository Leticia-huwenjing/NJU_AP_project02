#include "mapconfig.h"

/* Begin-1 起始点 End-0 终点
 * flyRoad-2 飞行路径 land-3 近战格子与地面路径同在land
 * remote-4 远程格子 barrier-5 不能部署 water-6 非路径-7(在config中最后一块给出)
*/

MapConfig::MapConfig(QString path) {
    //初始化map和path，金钱、生命值、敌人总数
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray read = file.readLine();
    QString trans = QString(read);
    money = trans.toInt();
    read = file.readLine();
    trans = QString(read);
    enemyNumber = trans.toInt();
    read = file.readLine();
    trans = QString(read);
    map.resize(trans.toInt());
    read = file.readLine();
    trans = QString(read);
    for(int i = 0; i < map.size(); i++){
        map[i].resize(trans.toInt());
    }
    for(int i = 0; i < map.size(); i++) {
        read = file.readLine();
        trans = QString(read);
        for(int j = 0; j < map[i].size(); j++) {
            Lattice* cell = new Lattice(QString(trans[j]).toInt(), j, i);
            map[i][j] = cell;
        }
    }
    for (int i = 0; i < map.size(); i++) {
        read = file.readLine();
        trans = QString(read);
        paths.append(trans.toInt());
    }

    //初始化不同类别敌人数量
    /* 将总蜜蜂分配到四种蜜蜂上
     * 根据地图比例分配蜜蜂总数,其中road与fly的分配按照路线数目分，
     * bees与tank按照5:2分配
     * flyA与flyB按照1:1分配
     */
    int flyPathNumber=0;
    for (int i = 0; i < paths.size(); i++) {
        if (paths[i] == 2) {
            flyPathNumber++;
        }
    }
    int flyNumber=0;
    flyNumber = (int)(((double)flyPathNumber/paths.size())*enemyNumber);
    flyANumber = flyNumber/2;
    flyBNumber = flyNumber-flyANumber;
    int landNumber = enemyNumber-flyNumber;
    beesNumber = landNumber*5/7;
    tankNumber = landNumber-beesNumber;
}

QVector<QVector<Lattice*>>& MapConfig::getMap() {
    return map;
}
QVector<int>& MapConfig::getPaths() {
    return paths;
}
int MapConfig::getMoney() {
    return money;
}
int MapConfig::getEnemyNumber() {
    return enemyNumber;
}
int MapConfig::getBeesNumber() {
    return beesNumber;
}
int MapConfig::getTanksNumber() {
    return tankNumber;
}
int MapConfig::getFlyANumber() {
    return flyANumber;
}
int MapConfig::getFlyBNumber() {
    return flyBNumber;
}
