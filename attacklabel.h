#ifndef ATTACKLABEL_H
#define ATTACKLABEL_H


#include <QLabel>
#include <QTimer>

class AttackLabel : public QLabel{
public:
    AttackLabel(QWidget* parent);
private:
    QTimer* timer;
};

#endif // ATTACKLABEL_H
