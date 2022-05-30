#include "state.h"

/* Begin-1 起始点 End-0 终点
 * flyRoad-2 飞行路径 land-3 近战格子与地面路径同在land
 * remote-4 远程格子 barrier-5 不能部署 water-6 非路径-7(在config中最后一块给出)
*/
State::State(QString path) {
    this->resize(1000, 650);
    //初始化地图
    MapConfig* config = new MapConfig(path);
    money = config->getMoney();
    enemyNumber = config->getEnemyNumber();
    beesNumber = enemyNumber/3;
    tankNumber = enemyNumber/6;
    flyANumber = enemyNumber/4;
    flyBNumber = enemyNumber-beesNumber-tankNumber-flyANumber;
    map = config->getMap();
    paths = config->getPaths();
    //将lattice的parent设置为this
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            map[i][j]->setParent(this);
        }
    }

    //初始化计时器
    timer = new QTimer(this);
    drawTimer = new QTimer(this);
    sunTimer = new QTimer(this);
}

void State::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawMap(painter);
    drawTowerScope(painter);
    drawAttack(painter);
}

void State::drawMap(QPainter& painter) {
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/main-background.png"));
    QVector<QString> skyPictures(3);
    skyPictures[0] = ":/sky1.png";
    skyPictures[1] = ":/sky2.png";
    skyPictures[2] = ":/sky3.png";
    QVector<QString> groundPictures(3);
    groundPictures[0] = ":/ground1.png";
    groundPictures[1] = ":/ground2.png";
    groundPictures[2] = ":/ground3.png";
    int sky=0, ground=0;
    for (int i = 0; i < map.size(); i++) {
        sky=0;
        ground=0;
        for (int j = 0; j < map[i].size(); j++) {
            map[i][j]->move(j*SIZE, i*SIZE);
            map[i][j]->resize(SIZE, SIZE);
            if (map[i][j]->getType() == 6) {
                painter.drawPixmap(j*SIZE, i*SIZE, W, H, QPixmap(skyPictures[sky]));
                sky = (sky+1)%3;
                painter.drawPixmap(j*SIZE, i*SIZE+H, W, H, QPixmap(":/water.png"));
            } else if (map[i][j]->getType() == 5) {
                painter.drawPixmap(j*SIZE, i*SIZE, SIZE, SIZE, QPixmap(":/block.png"));
            } else {
                painter.drawPixmap(j*SIZE, i*SIZE, W, H, QPixmap(skyPictures[sky]));
                sky = (sky+1)%3;
                painter.drawPixmap(j*SIZE, i*SIZE+H, W, H, QPixmap(groundPictures[ground]));
                ground = (ground+1)%3;
            }
        }
    }
}

void State::drawTowerScope(QPainter &painter) {
    painter.setPen(QPen(Qt::white, 2));
    for (auto& tower : BodyguardAnts) { //显示保护范围
        if (tower->isAlive && tower->detail) {
            painter.drawEllipse(tower->y_*SIZE, tower->x_*SIZE, SIZE, SIZE);
        }
    }
    for (auto& tower : NinjaAnts) {
        if (tower->isAlive && tower->detail) {
            painter.drawEllipse(tower->y_*SIZE, tower->x_*SIZE, SIZE, SIZE);
        }
    }
    for (auto& tower : FireAnts) {
        if (tower->isAlive && tower->detail) {
            painter.drawEllipse(tower->y_*SIZE-SIZE, tower->x_*SIZE-SIZE, 3*SIZE, 3*SIZE);
        }
    }
    for (auto& tower : ThrowerAnts) {
        if (tower->isAlive && tower->detail) {
            painter.drawEllipse(tower->y_*SIZE-SIZE, tower->x_*SIZE-SIZE, 3*SIZE, 3*SIZE);
        }
    }
    for (auto& tower : ScubaAnts) {
        if (tower->isAlive && tower->detail) {
            painter.drawEllipse(tower->y_*SIZE-SIZE, tower->x_*SIZE-SIZE, 3*SIZE, 3*SIZE);
        }
    }
    for (auto& tower : SlowAnts) {
        if (tower->isAlive && tower->detail) {
            painter.drawEllipse(tower->y_*SIZE-2*SIZE, tower->x_*SIZE-2*SIZE, 5*SIZE, 5*SIZE);
        }
    }
}

void State::drawAttack(QPainter &painter) {
    //bee
    painter.setPen(QPen(QColor(255, 215, 0), 3));
    for (auto& bee : Bees) {
        if (bee->target != nullptr && bee->isAction) {
            painter.drawLine(QPoint(bee->x(), bee->y()+30), QPoint(bee->target->x()+40, bee->target->y()));
        }
    }

    //flyA
    painter.setPen(QPen(QColor(139, 58, 58), 3));
    for (auto& flyA : AggressiveFlys) {
        if (flyA->target != nullptr && flyA->isAction) {
            painter.drawLine(QPoint(flyA->x(), flyA->y()+30), QPoint(flyA->target->x()+40, flyA->target->y()));
        }
    }

    //flyB
    painter.setPen(QPen(QColor(34, 139, 34), 3));
    for (auto& flyB : ScaryFlys) {
        if (flyB->target != nullptr && flyB->isAction) {
            painter.drawLine(QPoint(flyB->x(), flyB->y()+30), QPoint(flyB->target->x()+40, flyB->target->y()));
        }
    }

    //slowAnt
    painter.setPen(QPen(QColor(0, 250, 154), 3));
    for (auto& slowAnt : SlowAnts) {
        if (slowAnt->target != nullptr && slowAnt->isAction) {
            painter.drawLine(QPoint(slowAnt->x()+80, slowAnt->y()+40), QPoint(slowAnt->target->x(), slowAnt->target->y()+40));
        }
    }

    //scubaAnt
    painter.setPen(QPen(QColor(0, 255, 255), 3));
    for (auto& scubaAnt : ScubaAnts) {
        if (scubaAnt->target != nullptr && scubaAnt->isAction) {
            painter.drawLine(QPoint(scubaAnt->x()+80, scubaAnt->y()+40), QPoint(scubaAnt->target->x(), scubaAnt->target->y()+40));
        }
    }

    //throwerAnt
    painter.setPen(QPen(QColor(139, 115, 85), 3));
    for (auto& throwerAnt : ThrowerAnts) {
        if (throwerAnt->target != nullptr && throwerAnt->isAction) {
            painter.drawLine(QPoint(throwerAnt->x()+80, throwerAnt->y()+40), QPoint(throwerAnt->target->x(), throwerAnt->target->y()+40));
        }
    }
}

void State::init() {
    interfaceInit();
    beesInit();
}

void State::interfaceInit() { //界面初始化
    //设置金钱、生命、怪物数状态栏
    QLabel *moneyLabel = new QLabel(this);
    moneyLabel->resize(100, 40);
    moneyLabel->move(50, map.size()*SIZE+50);
    moneyLabel->setText("金钱："+ QString::number(this->money));
    moneyLabel->setStyleSheet("background-color: white;");
    moneyLabel->setAlignment(Qt::AlignVCenter);
    QFont ft;
    ft.setBold(true);
    ft.setPointSize(18);
    moneyLabel->setFont(ft);
    moneyLabel->setObjectName("moneyLabel");
    moneyLabel->show();

    QLabel *enemyLabel = new QLabel(this);
    enemyLabel->resize(120, 40);
    enemyLabel->move(200, map.size()*SIZE+50);
    enemyLabel->setText("怪物数："+ QString::number(this->enemyNumber));
    enemyLabel->setStyleSheet("background-color: white;");
    enemyLabel->setAlignment(Qt::AlignVCenter);
    enemyLabel->setFont(ft);
    enemyLabel->setObjectName("enemyLabel");
    enemyLabel->show();

    //初始化皇后蚂蚁(割草机+治疗)
    for (int i = 0; i < map.size(); i++) {
        if (map[i][0]->getType() == 0) {
            QueenAnt* queenAnt = new QueenAnt(this, i, 0);
            queenAnt->setObjectName("queen"+QString::number(i));
            queenAnt->resize(SIZE, SIZE);
            queenAnt->move(0, i*SIZE);
            QMovie* movie = new QMovie(":/ant_queen.gif");
            queenAnt->setMovie(movie);
            movie->start();
            queenAnt->show();
            QueenAnts.append(queenAnt);
        }
    }

    //设置蚂蚁栏背景色
    QLabel* whiteBack = new QLabel(this);
    whiteBack->resize(3.5*SIZE,7*SIZE);
    whiteBack->move(map[0].size()*SIZE+40, 30);
    whiteBack->setStyleSheet("background-color: white;");
    whiteBack->show();

    //设置蚂蚁栏
    QLabel* label00 = new QLabel(this);
    label00->resize(SIZE, SIZE);
    label00->move(map[0].size()*SIZE+60, 40);
    label00->setPixmap(QPixmap(":/ant_harvester.gif"));
    label00->setScaledContents(true);
    label00->show();
    QPushButton* button00 = new QPushButton(this);
    button00->resize(SIZE, SIZE/2);
    button00->move(label00->x(), label00->y()+SIZE);
    button00->setText("Harvester");
    button00->show();
    button00->setObjectName("tower00");

    QLabel* label01 = new QLabel(this);
    label01->resize(SIZE, SIZE);
    label01->move(map[0].size()*SIZE+60+SIZE*2, 40);
    label01->setPixmap(QPixmap(":/ant_thrower.gif"));
    label01->setScaledContents(true);
    label01->show();
    QPushButton* button01 = new QPushButton(this);
    button01->resize(SIZE, SIZE/2);
    button01->move(label01->x(), label01->y()+SIZE);
    button01->setText("Thrower");
    button01->show();
    button01->setObjectName("tower01");

    QLabel* label10 = new QLabel(this);
    label10->resize(SIZE, SIZE);
    label10->move(map[0].size()*SIZE+60, 40+SIZE*1.5);
    label10->setPixmap(QPixmap(":/ant_scuba.gif"));
    label10->setScaledContents(true);
    label10->show();
    QPushButton* button10 = new QPushButton(this);
    button10->resize(SIZE, SIZE/2);
    button10->move(label10->x(), label10->y()+SIZE);
    button10->setText("Scuba");
    button10->show();
    button10->setObjectName("tower10");

    QLabel* label11 = new QLabel(this);
    label11->resize(SIZE, SIZE);
    label11->move(map[0].size()*SIZE+60+SIZE*2, 40+SIZE*1.5);
    label11->setPixmap(QPixmap(":/ant_wall.gif"));
    label11->setScaledContents(true);
    label11->show();
    QPushButton* button11 = new QPushButton(this);
    button11->resize(SIZE, SIZE/2);
    button11->move(label11->x(), label11->y()+SIZE);
    button11->setText("Wall");
    button11->show();
    button11->setObjectName("tower11");

    QLabel* label20 = new QLabel(this);
    label20->resize(SIZE, SIZE);
    label20->move(map[0].size()*SIZE+60, 40+SIZE*3);
    label20->setPixmap(QPixmap(":/ant_slow.gif"));
    label20->setScaledContents(true);
    label20->show();
    QPushButton* button20 = new QPushButton(this);
    button20->resize(SIZE, SIZE/2);
    button20->move(label20->x(), label20->y()+SIZE);
    button20->setText("Slow");
    button20->show();
    button20->setObjectName("tower20");

    QLabel* label21 = new QLabel(this);
    label21->resize(SIZE, SIZE);
    label21->move(map[0].size()*SIZE+60+SIZE*2, 40+SIZE*3);
    label21->setPixmap(QPixmap(":/ant_ninja.gif"));
    label21->setScaledContents(true);
    label21->show();
    QPushButton* button21 = new QPushButton(this);
    button21->resize(SIZE, SIZE/2);
    button21->move(label21->x(), label21->y()+SIZE);
    button21->setText("Ninja");
    button21->show();
    button21->setObjectName("tower21");

    QLabel* label30 = new QLabel(this);
    label30->resize(SIZE, SIZE);
    label30->move(map[0].size()*SIZE+60, 40+SIZE*4.5);
    label30->setPixmap(QPixmap(":/ant_bodyguard.gif"));
    label30->setScaledContents(true);
    label30->show();
    QPushButton* button30 = new QPushButton(this);
    button30->resize(SIZE, SIZE/2);
    button30->move(label30->x(), label30->y()+SIZE);
    button30->setText("Bodyguard");
    button30->show();
    button30->setObjectName("tower30");

    QLabel* label31 = new QLabel(this);
    label31->resize(SIZE, SIZE);
    label31->move(map[0].size()*SIZE+60+SIZE*2, 40+SIZE*4.5);
    label31->setPixmap(QPixmap(":/ant_fire.gif"));
    label31->setScaledContents(true);
    label31->show();
    QPushButton* button31 = new QPushButton(this);
    button31->resize(SIZE, SIZE/2);
    button31->move(label31->x(), label31->y()+SIZE);
    button31->setText("Fire");
    button31->show();
    button31->setObjectName("tower31");

    //设置移除蚂蚁
    QPushButton* removeAnt = new QPushButton(this);
    removeAnt->resize(SIZE*1.5, SIZE/2);
    removeAnt->move((button30->x()+button31->x())/2-20, button30->y()+SIZE/2+10);
    removeAnt->setText("Remove Ant");
    removeAnt->show();
    removeAnt->setObjectName("removeAnt");

    //设置退出游戏
    QPushButton* endGame = new QPushButton(this);
    endGame->resize(SIZE, SIZE/2);
    endGame->move(400, map.size()*SIZE+50);
    endGame->setText("退出该局");
    endGame->show();
    endGame->setObjectName("endGame");

    //设置暂停游戏
    QPushButton* stopGame = new QPushButton(this);
    stopGame->resize(SIZE, SIZE/2);
    stopGame->move(500, map.size()*SIZE+50);
    stopGame->setText("暂停游戏");
    stopGame->show();
    stopGame->setObjectName("stopGame");

}

void State::beesInit() { //随机初始化蜜蜂
    int tmp=0;
    //生成四种相应数量的蜜蜂 时间到了再调用show

    for (int i = 0; i < flyANumber; i++) {
        tmp++;
        tmp %= paths.size();
        while (paths[tmp] != 2) {
            tmp++;
            tmp %= paths.size();
        }
        AggressiveFly* cur = new AggressiveFly(this, 2000*(i+1), map[0].size(), tmp);
        AggressiveFlys.append(cur);
        cur->move(SIZE*map[0].size(), SIZE*tmp);
        cur->setPixmap(QPixmap(":/FlyA.png"));
    }

    for (int i = 0; i < flyBNumber; i++) {
        tmp++;
        tmp %= paths.size();
        while (paths[tmp] != 2) {
            tmp++;
            tmp %= paths.size();
        }
        ScaryFly* cur = new ScaryFly(this, 2000*(i+1)+8000, map[0].size(), tmp);
        ScaryFlys.append(cur);
        cur->move(SIZE*map[0].size(), SIZE*tmp);
        cur->setPixmap(QPixmap(":/FlyB.png"));
    }

    for (int i = 0; i < beesNumber; i++) {
        tmp++;
        tmp %= paths.size();
        QMovie* m = new QMovie(":/bee.gif");
        while (paths[tmp] != 3) {
            tmp++;
            tmp %= paths.size();
        }
        Bee* cur = new Bee(this, 3000*(i+1), map[0].size(), tmp);
        Bees.append(cur);
        cur->move(SIZE*map[0].size(), SIZE*tmp);
        cur->setMovie(m);
        m->start();
    }

    for (int i = 0; i < tankNumber; i++) {
        tmp++;
        tmp %= paths.size();
        QMovie* m = new QMovie(":/bee_tank.gif");
        while (paths[tmp] != 3) {
            tmp++;
            tmp %= paths.size();
        }
        TankAnt* cur = new TankAnt(this, 5000*(i+1)+5000, map[0].size(), tmp);
        TankAnts.append(cur);
        cur->move(SIZE*map[0].size(), SIZE*tmp);
        cur->setMovie(m);
        m->start();
    }
}

void State::throwMoney() { //模拟随机掉落阳光
    int min = 0;
    int max = map[0].size();
    quint32 seed = quint32(QDateTime::currentDateTime().toMSecsSinceEpoch());
    QRandomGenerator generator(seed);
    int res = generator.bounded(min, max);
    Sun* curSun = new Sun(this);
    curSun->move(res*SIZE, 0);
    curSun->show();
    QPropertyAnimation* sunCartoon = new QPropertyAnimation(curSun, "geometry");
    sunCartoon->setDuration(4000);
    sunCartoon->setStartValue(QRect(res*SIZE, 0, 80, 80));
    sunCartoon->setEndValue(QRect(res*SIZE, map.size()*SIZE, 80, 80));
    sunCartoon->start();
    Suns.append(curSun);
}

//关联按键功能
void State::connectButtons() {
    //关联结束游戏功能
    connect(this->findChild<QPushButton*>("endGame"), &QPushButton::clicked, this, [=](){
        this->stop();
        QMessageBox::StandardButton res = QMessageBox::question(this, "EndGameQuestion", "您确定结束游戏吗？",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (res == QMessageBox::Yes) {
            delete this;
        } else {
            this->recover();
        }
    });

    //关联暂停游戏功能
    connect(this->findChild<QPushButton*>("stopGame"), &QPushButton::clicked, this, [=](){
        if (this->findChild<QPushButton*>("stopGame")->text() == "暂停游戏") {
            this->stop();
            this->findChild<QPushButton*>("stopGame")->setText("恢复游戏");
        } else {
            this->recover();
            this->findChild<QPushButton*>("stopGame")->setText("暂停游戏");
        }
    });

    //关联timer计时
    connect(timer, &QTimer::timeout, this, [=](){
        if (win) {
            stop();
            QMessageBox::information(this, "WinInformation", "祝贺您胜利！");
            close();
            QWidget* moneyWidget = new QWidget();
            moneyWidget->resize(680, 600);
            QLabel* Alipay = new QLabel(moneyWidget);
            Alipay->setScaledContents(true);
            Alipay->move(50, 50);
            Alipay->resize(270, 370);
            Alipay->setPixmap(QPixmap(":/Alipay.jpg"));
            Alipay->show();

            QLabel* Wechat = new QLabel(moneyWidget);
            Wechat->setScaledContents(true);
            Wechat->move(370, 50);
            Wechat->resize(270, 370);
            Wechat->setPixmap(QPixmap(":/Wechat.png"));
            Wechat->show();

            QLabel* head1 = new QLabel(moneyWidget);
            head1->setScaledContents(true);
            head1->move(100, 400);
            head1->resize(100, 100);
            head1->setPixmap(QPixmap(":/head.jpg"));
            head1->show();

            QLabel* head2 = new QLabel(moneyWidget);
            head2->setScaledContents(true);
            head2->move(240, 450);
            head2->resize(100, 100);
            QMovie* cat1 = new QMovie(":/cat1.gif");
            head2->setMovie(cat1);
            cat1->start();
            head2->show();

            QLabel* head3 = new QLabel(moneyWidget);
            head3->setScaledContents(true);
            head3->move(390, 400);
            head3->resize(100, 100);
            QMovie* cat2 = new QMovie(":/cat2.gif");
            head3->setMovie(cat2);
            cat2->start();
            head3->show();
            moneyWidget->show();

            QLabel* head4 = new QLabel(moneyWidget);
            head4->setScaledContents(true);
            head4->move(540, 450);
            head4->resize(100, 100);
            head4->setPixmap(QPixmap(":/huaji.jpg"));
            head4->show();
        } else if(gameOver) {
            stop();
            qDebug() << "剩余怪物数：" << enemyNumber;
            qDebug() << "bee：" << beesNumber;
            qDebug() << "tankAnt:" << tankNumber;
            qDebug() << "flyA:" << flyANumber;
            qDebug() << "flyB:" << flyBNumber;
            //debug
            qDebug() << "bee分割线-------------------";
            for (auto& bee : Bees) {
                qDebug() << bee->isShowed << bee->isAlive << bee->blood << bee->isHidden();
            }
            qDebug() << "tankAnt分割线-------------------";
            for (auto& tankAnt : TankAnts) {
                qDebug() << tankAnt->isShowed << tankAnt->isAlive << tankAnt->blood << tankAnt->isHidden();
            }
            qDebug() << "flyA分割线-------------------";
            for (auto& flyA : AggressiveFlys) {
                qDebug() << flyA->isShowed << flyA->isAlive << flyA->blood << flyA->isHidden();
            }
            qDebug() << "flyB分割线-------------------";
            for (auto& flyB : ScaryFlys) {
                qDebug() << flyB->isShowed << flyB->isAlive << flyB->blood << flyB->isHidden();
            }
            QMessageBox::information(this, "LoseInformation", "很遗憾您失败了！");
            close();
            QWidget* moneyWidget = new QWidget();
            moneyWidget->resize(680, 600);
            QLabel* Alipay = new QLabel(moneyWidget);
            Alipay->setScaledContents(true);
            Alipay->move(50, 50);
            Alipay->resize(270, 370);
            Alipay->setPixmap(QPixmap(":/Alipay.jpg"));
            Alipay->show();

            QLabel* Wechat = new QLabel(moneyWidget);
            Wechat->setScaledContents(true);
            Wechat->move(370, 50);
            Wechat->resize(270, 370);
            Wechat->setPixmap(QPixmap(":/Wechat.png"));
            Wechat->show();

            QLabel* head1 = new QLabel(moneyWidget);
            head1->setScaledContents(true);
            head1->move(100, 400);
            head1->resize(100, 100);
            head1->setPixmap(QPixmap(":/head.jpg"));
            head1->show();

            QLabel* head2 = new QLabel(moneyWidget);
            head2->setScaledContents(true);
            head2->move(240, 450);
            head2->resize(100, 100);
            QMovie* cat1 = new QMovie(":/cat1.gif");
            head2->setMovie(cat1);
            cat1->start();
            head2->show();

            QLabel* head3 = new QLabel(moneyWidget);
            head3->setScaledContents(true);
            head3->move(390, 400);
            head3->resize(100, 100);
            QMovie* cat2 = new QMovie(":/cat2.gif");
            head3->setMovie(cat2);
            cat2->start();
            head3->show();
            moneyWidget->show();

            QLabel* head4 = new QLabel(moneyWidget);
            head4->setScaledContents(true);
            head4->move(540, 450);
            head4->resize(100, 100);
            head4->setPixmap(QPixmap(":/huaji.jpg"));
            head4->show();
        } else {
            updateStatus();
        }
    });

    //关联重新绘图
    connect(drawTimer, &QTimer::timeout, this, [=](){
        update();

        //关联新的塔与点击塔事件的关系,不会被暂停
        connectTower();

        //点击阳光增加金钱,不会被暂停！
        for (auto& sun : Suns) {
            if (sun->isClicked && !sun->iscnt) {
                sun->iscnt = true;
                money += 50;
                this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            }
        }
    });

    //关联产生新阳光
    connect(sunTimer, &QTimer::timeout, this, [=](){
        sunTimer->setInterval(5000);
        throwMoney();
    });

    //关联添加新蚂蚁信号触发
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            QPushButton* curTower = this->findChild<QPushButton*>(QString("tower%1%2").arg(i).arg(j));
            connect(curTower , &QPushButton::clicked, this, [=](){
                if (i == 0 && j == 0) {
                    pTower = new HarvesterAnt(this, x, y);
                }
                if (i == 0 && j == 1) {
                    pTower = new ThrowerAnt(this, x, y);
                }
                if (i == 1 && j == 0) {
                    pTower = new ScubaAnt(this, x, y);
                }
                if (i == 1 && j == 1) {
                    pTower = new WallAnt(this, x, y);
                }
                if (i == 2 && j == 0) {
                    pTower = new SlowAnt(this, x, y);
                }
                if (i == 2 && j == 1) {
                    pTower = new NinjaAnt(this, x, y); //x,y是矩阵坐标
                }
                if (i == 3 && j == 0) {
                    if (pTower != nullptr) {
                        if (pTower->getType() != 7) {
                            pTower = new BodyguardAnt(this, x, y);
                        }
                    } else {
                        pTower = new BodyguardAnt(this, x, y);
                    }
                }
                if (i == 3 && j == 1) {
                    pTower = new FireAnt(this, x, y);
                }
                emit wantAdd();
            });
        }
    }

    //捕获鼠标正处于哪一个格子处
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            Lattice* curLattice = map[i][j];
            connect(curLattice, &Lattice::addTower, this, [=](){
                getPutPos(curLattice->getY(), curLattice->getX()); //得到矩阵坐标
            });
        }
    }

    connect(this, &State::wantAdd, this, [=](){
        addTower(pTower);
        pTower = nullptr;
    });

    connect(this->findChild<QPushButton*>("removeAnt"), &QPushButton::clicked, this, [=](){
        removeTower(pTower);
        pTower = nullptr;
    });
}

void State::connectTower() { //Todo
    for (auto& tower : BodyguardAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
    for (auto& tower : FireAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
    for (auto& tower : HarvesterAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
    for (auto& tower : NinjaAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
    for (auto& tower : ScubaAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
    for (auto& tower : SlowAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
    for (auto& tower : ThrowerAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
    for (auto& tower : WallAnts) {
        connect(tower, &Tower::chooseTower, this, [=](){
            getPutPos(tower->x_, tower->y_);
            pTower = tower;
        });
    }
}

bool State::addTower(Tower* curTower) {
    bool res = false;
    if(curTower == nullptr || x < 0 || y < 0) {
        return false;
    }

    if (map[x][y]->getType() == 4 && curTower->getType() == 1) { //harvester
        if (curTower->getCost() <= money) {
            HarvesterAnts.append((HarvesterAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_harvester.gif");
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    if (map[x][y]->getType() == 4 && curTower->getType() == 2) { //thrower
        if (curTower->getCost() <= money) {
            ThrowerAnts.append((ThrowerAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_thrower.gif");
            curTower->setScaledContents(true);
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    if (map[x][y]->getType() == 6 && curTower->getType() == 3) { //scuba
        if (curTower->getCost() <= money) {
            ScubaAnts.append((ScubaAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_scuba.gif");
            curTower->setScaledContents(true);
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    if (map[x][y]->getType() == 3 && curTower->getType() == 4) { //wall
        if (curTower->getCost() <= money) {
            WallAnts.append((WallAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_wall.gif");
            curTower->setScaledContents(true);
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    if (map[x][y]->getType() == 4 && curTower->getType() == 5) { //slow
        if (curTower->getCost() <= money) {
            SlowAnts.append((SlowAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_slow.gif");
            curTower->setScaledContents(true);
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    if (map[x][y]->getType() == 3 && curTower->getType() == 6) { //Ninja
        if (curTower->getCost() <= money) {
            NinjaAnts.append((NinjaAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_ninja.gif");
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    if ((map[x][y]->getType() == 3 || map[x][y]->getType() == 4) && curTower->getType() == 7) { //bodyguard
        if (curTower->getCost() <= money) {
            BodyguardAnts.append((BodyguardAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_bodyguard.gif");
            curTower->setScaledContents(true);
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    if(map[x][y]->getType() == 3 && curTower->getType() == 8) { //fire
        if (curTower->getCost() <= money) {
            FireAnts.append((FireAnt*)curTower);
            QMovie* mov = new QMovie(":/ant_fire.gif");
            curTower->setScaledContents(true);
            curTower->resize(SIZE, SIZE);
            curTower->move(y*SIZE, x*SIZE);
            curTower->setMovie(mov);
            mov->start();
            curTower->show();
            money -= curTower->getCost();
            this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
            res = true;
        }
    }

    x=-1;
    y=-1;
    pTower = nullptr;
    return res;
}

bool State::removeTower(Tower *curTower) {
    bool res = false;
    if(curTower == nullptr) {
        return false;
    }
    money += curTower->getCompensate();
    this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
    curTower->die();

    for(auto& tower : BodyguardAnts) {
        qDebug() << tower->isAlive << tower->isHidden();
    }

    return res;
}

void State::getPutPos(int _x, int _y) {
    x = _x;
    y = _y;
}

void State::stop() {
    timer->stop();
    int sunRemainTime = sunTimer->remainingTime();
    sunTimer->stop();
    sunTimer->setInterval(sunRemainTime);
    for (auto& bee : Bees) {
        bee->stop();
    }
    for (auto& tankAnt : TankAnts) {
        tankAnt->stop();
    }
    for (auto& flyA : AggressiveFlys) {
        flyA->stop();
    }
    for (auto& flyB : ScaryFlys) {
        flyB->stop();
    }
    for (auto& tower : BodyguardAnts) {
        tower->stop();
    }
    for (auto& tower : HarvesterAnts) {
        tower->stop();
    }
    for (auto& tower : FireAnts) {
        tower->stop();
    }
    for (auto& tower : NinjaAnts) {
        tower->stop();
    }
    for (auto& tower : ScubaAnts) {
        tower->stop();
    }
    for (auto& tower : SlowAnts) {
        tower->stop();
    }
    for (auto& tower : ThrowerAnts) {
        tower->stop();
    }
    for (auto& tower : WallAnts) {
        tower->stop();
    }
}

void State::recover() {
    timer->start();
    sunTimer->start();
    for (auto& bee : Bees) {
        bee->recover();
    }
    for (auto& tankAnt : TankAnts) {
        tankAnt->recover();
    }
    for (auto& flyA : AggressiveFlys) {
        flyA->recover();
    }
    for (auto& flyB : ScaryFlys) {
        flyB->recover();
    }
    for (auto& tower : BodyguardAnts) {
        tower->recover();
    }
    for (auto& tower : HarvesterAnts) {
        tower->recover();
    }
    for (auto& tower : FireAnts) {
        tower->recover();
    }
    for (auto& tower : NinjaAnts) {
        tower->recover();
    }
    for (auto& tower : ScubaAnts) {
        tower->recover();
    }
    for (auto& tower : SlowAnts) {
        tower->recover();
    }
    for (auto& tower : ThrowerAnts) {
        tower->recover();
    }
    for (auto& tower : WallAnts) {
        tower->recover();
    }
}

void State::updateStatus() {
    //harvester产出金钱
    for (auto& harvesterAnt : HarvesterAnts) {
        if (harvesterAnt->isAlive) {
            if (harvesterAnt->haveMoney) {
                if (harvesterAnt->curSun != nullptr && harvesterAnt->curSun->isClicked) {
                    money += 50;
                    this->findChild<QLabel*>("moneyLabel")->setText("金钱：" + QString::number(money));
                    harvesterAnt->haveMoney = false;
                    harvesterAnt->curSun = nullptr;
                }
            }
        }
    }

    //NinjaAnt设置目标(我方近战单位)
    for (auto& ninjaAnt : NinjaAnts) {
        ninjaAnt->target = nullptr; //重置ninjaAnt的target
        if (ninjaAnt->isAlive) {
            for (auto& bee : Bees) { //遍历蜜蜂
                if (bee->isAlive) {
                    if (ninjaAnt->x_ == bee->x_ && ninjaAnt->y_ == bee->y_) {
                        ninjaAnt->target = bee;
                        break;
                    }
                }
            }
            if (ninjaAnt->target == nullptr) {
                for (auto& tankAnt : TankAnts) { //遍历坦克蚂蚁
                    if (tankAnt->isAlive) {
                        if (ninjaAnt->x_ == tankAnt->x_ && ninjaAnt->y_ == tankAnt->y_) {
                            ninjaAnt->target = tankAnt;
                            break;
                        }
                    }
                }
            }
        }
    }


    for (auto& throwerAnt : ThrowerAnts) {
        throwerAnt->target = nullptr; //重置throwerAnt的target
        if (throwerAnt->isAlive) {
            for (auto& flyB : ScaryFlys) {
                if (flyB->isAlive) {
                    if (qAbs(throwerAnt->x_ - flyB->x_) <= 1 && qAbs(throwerAnt->y_ - flyB->y_) <= 1) {
                        throwerAnt->target = flyB;
                        break;
                    }
                }
            }
            if (throwerAnt->target == nullptr) {
                for (auto& flyA : AggressiveFlys) {
                    if (flyA->isAlive) {
                        if (qAbs(throwerAnt->x_ - flyA->x_) <= 1 && qAbs(throwerAnt->y_ - flyA->y_) <= 1) {
                            throwerAnt->target = flyA;
                            break;
                        }
                    }
                }
            }
            if (throwerAnt->target == nullptr) {
                for (auto& tankAnt : TankAnts) {
                    if (tankAnt->isAlive) {
                        if (qAbs(throwerAnt->x_ - tankAnt->x_) <= 1 && qAbs(throwerAnt->y_ - tankAnt->y_) <= 1) {
                            throwerAnt->target = tankAnt;
                            break;
                        }
                    }
                }
            }
            if (throwerAnt->target == nullptr) {
                for (auto& bee : Bees) {
                    if (bee->isAlive) {
                        if (qAbs(throwerAnt->x_ - bee->x_) <= 1 && qAbs(throwerAnt->y_ - bee->y_) <= 1) {
                            throwerAnt->target = bee;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (auto& scubaAnt : ScubaAnts) {
        scubaAnt->target = nullptr; //重置scubaAnt的target
        if (scubaAnt->isAlive) {
            for (auto& flyB : ScaryFlys) {
                if (flyB->isAlive) {
                    if (qAbs(scubaAnt->x_ - flyB->x_) <= 1 && qAbs(scubaAnt->y_ - flyB->y_) <= 1) {
                        scubaAnt->target = flyB;
                        break;
                    }
                }
            }
            if (scubaAnt->target == nullptr) {
                for (auto& flyA : AggressiveFlys) {
                    if (flyA->isAlive) {
                        if (qAbs(scubaAnt->x_ - flyA->x_) <= 1 && qAbs(scubaAnt->y_ - flyA->y_) <= 1) {
                            scubaAnt->target = flyA;
                            break;
                        }
                    }
                }
            }
            if (scubaAnt->target == nullptr) {
                for (auto& tankAnt : TankAnts) {
                    if (tankAnt->isAlive) {
                        if (qAbs(scubaAnt->x_ - tankAnt->x_) <= 1 && qAbs(scubaAnt->y_ - tankAnt->y_) <= 1) {
                            scubaAnt->target = tankAnt;
                            break;
                        }
                    }
                }
            }
            if (scubaAnt->target == nullptr) {
                for (auto& bee : Bees) {
                    if (bee->isAlive) {
                        if (qAbs(scubaAnt->x_ - bee->x_) <= 1 && qAbs(scubaAnt->y_ - bee->y_) <= 1) {
                            scubaAnt->target = bee;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (auto& fireAnt : FireAnts) {
        if (fireAnt->isAlive) {
            while (fireAnt->targets.size() > 0) {
                fireAnt->targets.pop_back();
            }
            for (auto& flyB : ScaryFlys) {
                if (flyB->isAlive) {
                    if (qAbs(fireAnt->x_ - flyB->x_) <= 1 && qAbs(fireAnt->y_ - flyB->y_) <= 1) {
                        fireAnt->targets.append(flyB);
                    }
                }
            }
            for (auto& flyA : AggressiveFlys) {
                if (flyA->isAlive) {
                    if (qAbs(fireAnt->x_ - flyA->x_) <= 1 && qAbs(fireAnt->y_ - flyA->y_) <= 1) {
                        fireAnt->targets.append(flyA);
                    }
                }
            }

            for (auto& tankAnt : TankAnts) {
                if (tankAnt->isAlive) {
                    if (qAbs(fireAnt->x_ - tankAnt->x_) <= 1 && qAbs(fireAnt->y_ - tankAnt->y_) <= 1) {
                        fireAnt->targets.append(tankAnt);
                    }
                }
            }
            for (auto& bee : Bees) {
                if (bee->isAlive) {
                    if (qAbs(fireAnt->x_ - bee->x_) <= 1 && qAbs(fireAnt->y_ - bee->y_) <= 1) {
                        fireAnt->targets.append(bee);
                    }
                }
            }
        }
    }

    //SlowAnt设置目标(我方远程单位)
    for (auto& slowAnt : SlowAnts) {
        slowAnt->target = nullptr; //重置slowAnt的target
        if (slowAnt->isAlive) {
            for (auto& bee : Bees) { //遍历蜜蜂
                if (bee->isAlive) {
                    if (qAbs(slowAnt->x_ - bee->x_) <= 2 && qAbs(slowAnt->y_ - bee->y_) <= 2) {
                        slowAnt->target = bee;
                        break;
                    }
                }
            }
            if (slowAnt->target == nullptr) {
                for (auto& tankAnt : TankAnts) { //遍历坦克蚂蚁
                    if (tankAnt->isAlive) {
                        if (qAbs(slowAnt->x_ - tankAnt->x_) <= 2 && qAbs(slowAnt->y_ - tankAnt->y_) <= 2) {
                            slowAnt->target = tankAnt;
                            break;
                        }
                    }
                }
            }
        }
    }

    //TankAnt设置目标(敌方近程)
    for (auto& tankAnt : TankAnts) {
        tankAnt->target = nullptr; //重置tankAnt的target
        if (tankAnt->isAlive) {
            for (auto& bodyguardAnt : BodyguardAnts) { //优先攻击保护蚂蚁
                if (bodyguardAnt->isAlive) {
                    if (bodyguardAnt->x_ == tankAnt->x_ && bodyguardAnt->y_ == tankAnt->y_) {
                        tankAnt->target = bodyguardAnt;
                        break;
                    }
                }
            }
            if (tankAnt->target == nullptr) {
                for (auto& wallAnt : WallAnts) {
                    if (wallAnt->isAlive) {
                        if (wallAnt->x_ == tankAnt->x_ && wallAnt->y_ == tankAnt->y_) {
                            tankAnt->target = wallAnt;
                            break;
                        }
                    }
                }
            }
            if (tankAnt->target == nullptr) {
                for (auto& ninjaAnt : NinjaAnts) {
                    if (ninjaAnt->isAlive) {
                        if (ninjaAnt->x_ == tankAnt->x_ && ninjaAnt->y_ == tankAnt->y_) {
                            tankAnt->target = ninjaAnt;
                            break;
                        }
                    }
                }
            }
            if (tankAnt->target == nullptr) {
                for (auto& scubaAnt : ScubaAnts) {
                    if (scubaAnt->isAlive) {
                        if (scubaAnt->x_ == tankAnt->x_ && scubaAnt->y_ == tankAnt->y_) {
                            tankAnt->target = scubaAnt;
                            break;
                        }
                    }
                }
            }
        }
    }


    //Bee设置目标(敌方地面远程) bee的射程为含自己所在格子一共向前3个身位格，含上下
    for (auto& bee : Bees) {
        bee->target = nullptr;
        if (bee->isAlive) {
            for (auto& bodyguardAnt : BodyguardAnts) {
                if (bodyguardAnt->isAlive) {
                    if (qAbs(bodyguardAnt->x_ - bee->x_) <= 1 && bee->y_ - bodyguardAnt->y_ >= 0 && bee->y_ - bodyguardAnt->y_ <= 2) {
                        bee->target = bodyguardAnt;
                        break;
                    }
                }
            }
            if (bee->target == nullptr) {
                for (auto& wallAnt : WallAnts) {
                    if (wallAnt->isAlive) {
                        if (qAbs(wallAnt->x_ - bee->x_) <= 1 && bee->y_ - wallAnt->y_ >= 0 && bee->y_ - wallAnt->y_ <= 2) {
                            bee->target = wallAnt;
                            break;
                        }
                    }
                }
            }


            if (bee->target == nullptr) {
                for (auto& ninjaAnt : NinjaAnts) {
                    if (ninjaAnt->isAlive) {
                        if (qAbs(ninjaAnt->x_ - bee->x_) <= 1 && bee->y_ - ninjaAnt->y_ >= 0 && bee->y_ - ninjaAnt->y_ <= 2) {
                            bee->target = ninjaAnt;
                            break;
                        }
                    }
                }
            }

            if (bee->target == nullptr) {
                for (auto& scubaAnt : ScubaAnts) {
                    if (scubaAnt->isAlive) {
                        if (qAbs(scubaAnt->x_ - bee->x_) <= 1 && bee->y_ - scubaAnt->y_ >= 0 && bee->y_ - scubaAnt->y_ <= 2) {
                            bee->target = scubaAnt;
                            break;
                        }
                    }
                }
            }

            if (bee->target == nullptr) {
                for (auto& throwerAnt : ThrowerAnts) {
                    if (throwerAnt->isAlive) {
                        if (qAbs(throwerAnt->x_ - bee->x_) <= 1 && bee->y_ - throwerAnt->y_ >= 0 && bee->y_ - throwerAnt->y_ <= 2) {
                            bee->target = throwerAnt;
                            break;
                        }
                    }
                }
            }

            if (bee->target == nullptr) {
                for (auto& slowAnt : SlowAnts) {
                    if (slowAnt->isAlive) {
                        if (qAbs(slowAnt->x_ - bee->x_) <= 1 && bee->y_ - slowAnt->y_ >= 0 && bee->y_ - slowAnt->y_ <= 2) {
                            bee->target = slowAnt;
                            break;
                        }
                    }
                }
            }

            if (bee->target == nullptr) {
                for (auto& harvesterAnt : HarvesterAnts) {
                    if (harvesterAnt->isAlive) {
                        if (qAbs(harvesterAnt->x_ - bee->x_) <= 1 && bee->y_ - harvesterAnt->y_ >= 0 && bee->y_ - harvesterAnt->y_ <= 2) {
                            bee->target = harvesterAnt;
                            break;
                        }
                    }
                }
            }
        }
    }

    //flyA设置目标(好战的) 优先攻击远程
    for (auto& flyA : AggressiveFlys) {
        flyA->target = nullptr;
        if (flyA->isAlive) {
            for (auto& tower : BodyguardAnts) {
                if (tower->isAlive) {
                    if (qAbs(tower->x_ - flyA->x_) <= 1 && qAbs(tower->y_ - flyA->y_) <= 1) {
                        flyA->target = tower;
                        break;
                    }
                }
            }
            if (flyA->target == nullptr) {
                for (auto& tower : ThrowerAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyA->x_) <= 1 && qAbs(tower->y_ - flyA->y_) <= 1) {
                            flyA->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyA->target == nullptr) {
                for (auto& tower : SlowAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyA->x_) <= 1 && qAbs(tower->y_ - flyA->y_) <= 1) {
                            flyA->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyA->target == nullptr) {
                for (auto& tower : HarvesterAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyA->x_) <= 1 && qAbs(tower->y_ - flyA->y_) <= 1) {
                            flyA->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyA->target == nullptr) {
                for (auto& tower : WallAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyA->x_) <= 1 && qAbs(tower->y_ - flyA->y_) <= 1) {
                            flyA->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyA->target == nullptr) {
                for (auto& tower : NinjaAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyA->x_) <= 1 && qAbs(tower->y_ - flyA->y_) <= 1) {
                            flyA->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyA->target == nullptr) {
                for (auto& tower : ScubaAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyA->x_) <= 1 && qAbs(tower->y_ - flyA->y_) <= 1) {
                            flyA->target = tower;
                            break;
                        }
                    }
                }
            }

        }
    }

    //flyB设置目标(畏战的)
    for (auto& flyB : ScaryFlys) {
        flyB->target = nullptr;
        if (flyB->isAlive) {
            for (auto& tower : BodyguardAnts) {
                if (tower->isAlive) {
                    if (qAbs(tower->x_ - flyB->x_) <= 1 && qAbs(tower->y_ - flyB->y_) <= 1) {
                        flyB->target = tower;
                        break;
                    }
                }
            }
            if (flyB->target == nullptr) {
                for (auto& tower : ThrowerAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyB->x_) <= 1 && qAbs(tower->y_ - flyB->y_) <= 1) {
                            flyB->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyB->target == nullptr) {
                for (auto& tower : SlowAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyB->x_) <= 1 && qAbs(tower->y_ - flyB->y_) <= 1) {
                            flyB->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyB->target == nullptr) {
                for (auto& tower : HarvesterAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyB->x_) <= 1 && qAbs(tower->y_ - flyB->y_) <= 1) {
                            flyB->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyB->target == nullptr) {
                for (auto& tower : WallAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyB->x_) <= 1 && qAbs(tower->y_ - flyB->y_) <= 1) {
                            flyB->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyB->target == nullptr) {
                for (auto& tower : NinjaAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyB->x_) <= 1 && qAbs(tower->y_ - flyB->y_) <= 1) {
                            flyB->target = tower;
                            break;
                        }
                    }
                }
            }

            if (flyB->target == nullptr) {
                for (auto& tower : ScubaAnts) {
                    if (tower->isAlive) {
                        if (qAbs(tower->x_ - flyB->x_) <= 1 && qAbs(tower->y_ - flyB->y_) <= 1) {
                            flyB->target = tower;
                            break;
                        }
                    }
                }
            }

        }
    }

    //将所有生命值低于0的bee调用die
    for (auto& bee: Bees) {
        if (bee->isAlive && bee->blood <= 0) {
            bee->die();
            enemyNumber--;
        }
    }

    //将所有生命值低于0的tankAnt调用die
    for (auto& tankAnt : TankAnts) {
        if (tankAnt->isAlive && tankAnt->blood <= 0) {
            tankAnt->die();
            enemyNumber--;
        }
    }

    //将所有生命值低于0的flyA调用die
    for (auto& flyA : AggressiveFlys) {
        if (flyA->isAlive && flyA->blood <= 0) {
            flyA->die();
            enemyNumber--;
        }
    }

    //将所有生命值低于0的flyB调用die
    for (auto& flyB : ScaryFlys) {
        if (flyB->isAlive && flyB->blood <= 0) {
            flyB->die();
            enemyNumber--;
        }
    }

    //将所有生命值低于0的harvesterAnt调用die
    for (auto& harvesterAnt : HarvesterAnts) {
        if (harvesterAnt->isAlive && harvesterAnt->blood <= 0) {
            harvesterAnt->die();
        }
    }

    //将所有生命值低于0的scubaAnt调用die
    for (auto& scubaAnt : ScubaAnts) {
        if (scubaAnt->isAlive && scubaAnt->blood <= 0) {
            scubaAnt->die();
        }
    }

    //将所有生命值低于0的wallAnt调用die
    for (auto& wallAnt : WallAnts) {
        if (wallAnt->isAlive && wallAnt->blood <= 0) {
            wallAnt->die();
        }
    }

    //将所有生命值低于0的slowAnt调用die
    for (auto& slowAnt : SlowAnts) {
        if (slowAnt->isAlive && slowAnt->blood <= 0) {
            slowAnt->die();
        }
    }

    //将所有生命值低于0的bodyguardAnt调用die
    for (auto& bodyguardAnt : BodyguardAnts) {
        if (bodyguardAnt->isAlive && bodyguardAnt->blood <= 0) {
            bodyguardAnt->die();
        }
    }

    //将所有生命值低于0的fireAnt调用die
    for (auto& fireAnt : FireAnts) {
        if (fireAnt->isAlive && fireAnt->blood <= 0) {
            fireAnt->die();
        }
    }

    //将所有生命值低于0的ninjaAnt调用die
    for (auto& ninjaAnt : NinjaAnts) {
        if (ninjaAnt->isAlive && ninjaAnt->blood <= 0) {
            ninjaAnt->die();
        }
    }

    //将所有生命值低于0的throwerAnt调用die
    for (auto& throwerAnt : ThrowerAnts) {
        if (throwerAnt->isAlive && throwerAnt->blood <= 0) {
            throwerAnt->die();
        }
    }

    //判断皇后蚂蚁的位置是否有bee
    for (auto& bee: Bees) {
        if (bee->isAlive) {
            for (auto& queenAnt: QueenAnts) {
                if (queenAnt->isAlive) {
                    if (!queenAnt->isRaiseUp){
                        if (bee->x_ == queenAnt->x_ && bee->y_ == queenAnt->y_) {
                            queenAnt->raiseUp();
                        }
                    }
                }
            }
        }
    }

    //判断皇后蚂蚁的位置是否有tankAnt
    for (auto& tankAnt: TankAnts) {
        if (tankAnt->isAlive) {
            for (auto& queenAnt: QueenAnts) {
                if (queenAnt->isAlive) {
                    if (!queenAnt->isRaiseUp){
                        if (tankAnt->x_ == queenAnt->x_ && tankAnt->y_ == queenAnt->y_) {
                            queenAnt->raiseUp();
                        }
                    }
                }
            }
        }
    }

    //判断皇后蚂蚁的位置是否有flyA
    for (auto& flyA: AggressiveFlys) {
        if (flyA->isAlive) {
            for (auto& queenAnt: QueenAnts) {
                if (queenAnt->isAlive) {
                    if (!queenAnt->isRaiseUp){
                        if (flyA->x_ == queenAnt->x_ && flyA->y_ == queenAnt->y_) {
                            queenAnt->raiseUp();
                        }
                    }
                }
            }
        }
    }

    //判断皇后蚂蚁的位置是否有flyB
    for (auto& flyB: ScaryFlys) {
        if (flyB->isAlive) {
            for (auto& queenAnt: QueenAnts) {
                if (queenAnt->isAlive) {
                    if (!queenAnt->isRaiseUp){
                        if (flyB->x_ == queenAnt->x_ && flyB->y_ == queenAnt->y_) {
                            queenAnt->raiseUp();
                        }
                    }
                }
            }
        }
    }

    //已经激活的皇后蚂蚁运动到的地方的敌人全部死亡
    for (auto& queenAnt: QueenAnts) {
        if (queenAnt->isAlive && queenAnt->isRaiseUp) {
            for (auto& bee: Bees) {
                if (bee->isAlive) {
                    if (bee->x_ == queenAnt->x_ && bee->y_ == queenAnt->y_) {
                        bee->die();
                        enemyNumber--;
                    }
                }
            }
            for (auto& tankAnt: TankAnts) {
                if (tankAnt->isAlive) {
                    if (tankAnt->x_ == queenAnt->x_ && tankAnt->y_ == queenAnt->y_) {
                        tankAnt->die();
                        enemyNumber--;
                    }
                }
            }

            for (auto& flyA: AggressiveFlys) {
                if (flyA->isAlive) {
                    if (flyA->x_ == queenAnt->x_ && flyA->y_ == queenAnt->y_) {
                        flyA->die();
                        enemyNumber--;
                    }
                }
            }

            for (auto& flyB: ScaryFlys) {
                if (flyB->isAlive) {
                    if (flyB->x_ == queenAnt->x_ && flyB->y_ == queenAnt->y_) {
                        flyB->die();
                        enemyNumber--;
                    }
                }
            }

        }
    }

    //判断bee是否到达终点，如果到达终点且该终点没有皇后蚂蚁，则游戏结束
    for (auto& bee: Bees) {
        if (bee->isAlive) {
            if (bee->y_ == 0) {
                for (auto& queenAnt: QueenAnts) {
                    if (queenAnt->x_ == bee->x_ && !queenAnt->isAlive) {
                        gameOver = true;
                    }
                }
            }
        }
    }

    //判断tankAnt是否到达终点，如果到达终点且该终点没有皇后蚂蚁，则游戏结束
    for (auto& tankAnt: TankAnts) {
        if (tankAnt->isAlive) {
            if (tankAnt->y_ == 0) {
                for (auto& queenAnt: QueenAnts) {
                    if (queenAnt->x_ == tankAnt->x_ && !queenAnt->isAlive) {
                        gameOver = true;
                    }
                }
            }
        }
    }

    //判断flyA是否到达终点，如果到达终点且该终点没有皇后蚂蚁，则游戏结束
    for (auto& flyA: AggressiveFlys) {
        if (flyA->isAlive) {
            if (flyA->y_ == 0) {
                for (auto& queenAnt: QueenAnts) {
                    if (queenAnt->x_ == flyA->x_ && !queenAnt->isAlive) {
                        gameOver = true;
                    }
                }
            }
        }
    }

    //判断flyB是否到达终点，如果到达终点且该终点没有皇后蚂蚁，则游戏结束
    for (auto& flyB: ScaryFlys) {
        if (flyB->isAlive) {
            if (flyB->y_ == 0) {
                for (auto& queenAnt: QueenAnts) {
                    if (queenAnt->x_ == flyB->x_ && !queenAnt->isAlive) {
                        gameOver = true;
                    }
                }
            }
        }
    }

    //更新怪物数
    this->findChild<QLabel*>("enemyLabel")->setText("怪物数：" + QString::number(enemyNumber));

    //怪物数为0则胜利
    if (enemyNumber <= 0) {
        win = true;
    }
}

void State::play() {
    init();
    timer->start(30);
    drawTimer->start(30);
    sunTimer->start(5000);
    connectButtons();
}


