#include "attacklabel.h"

AttackLabel::AttackLabel(QWidget* parent) : QLabel(parent){
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, [=](){
        this->hide();
        timer->stop();
    });
}
