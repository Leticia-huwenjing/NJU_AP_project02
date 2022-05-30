#include "file.h"

File::File(QString path) {

}
QVector<QVector<Lattice*>>& File::getMap() {
    return map;
}
QVector<int>& File::getPaths() {
    return paths;
}
int File::getMoney() {
    return money;
}
int File::getLives() {
    return lives;
}
int File::getEnemyNumber() {
    return enemyNumber;
}
int File::getBeesNumber() {
    return beesNumber;
}
int File::getTanksNumber() {
    return tankNumber;
}
int File::getFlyANumber() {
    return flyANumber;
}
int File::getFlyBNumber() {
    return flyBNumber;
}
