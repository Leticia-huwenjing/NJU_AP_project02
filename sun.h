#ifndef SUN_H
#define SUN_H

#include <QLabel>
#include <QPixmap>
#include "tower.h"

class Sun : public QLabel{
public:
    Sun(QWidget* parent);
    void mousePressEvent(QMouseEvent* ev) override;
    bool isClicked=false;
    bool iscnt = false;
    QTimer* timer;
};

#endif // SUN_H
