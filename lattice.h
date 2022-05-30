#ifndef LATTICE_H
#define LATTICE_H


#include <QLabel>
#include <QMouseEvent>


class Lattice : public QLabel{
    Q_OBJECT
public:
        Lattice();
        Lattice(int _type, int _x, int _y);
        int getType();
        void mousePressEvent(QMouseEvent* ev) override;
        int getX();
        int getY();

signals:
        void addTower();

private:
        int type;
        int x, y; //横纵坐标！！！不是矩阵坐标
};

#endif // LATTICE_H


