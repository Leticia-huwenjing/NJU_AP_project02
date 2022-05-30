#include "lattice.h"

Lattice::Lattice()
{

}

Lattice::Lattice(int _type, int _x, int _y) {
    type = _type;
    x = _x;
    y = _y;
}

int Lattice::getType() {
    return type;
}


void Lattice::mousePressEvent(QMouseEvent* ev) {
    if (ev->button() == Qt::LeftButton) {
        qDebug() << "鼠标点击了(" << y << "," << x << ")";
        emit addTower();
    }
}

int Lattice::getX() {
    return x;
}

int Lattice::getY() {
    return y;
}
