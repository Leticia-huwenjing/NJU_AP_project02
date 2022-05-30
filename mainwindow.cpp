#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapPath = "";
    setButton();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/splash.png"));
}

//关联按键作用
void MainWindow::setButton(){
    connect(ui->exitButton, &QPushButton::clicked, this, [=](){
        this->close();
    });
    connect(ui->beginButton, &QPushButton::clicked, this, [=](){
        if (mapPath != "") {
            this->state = new State(mapPath);
            this->state->show();
            this->state->play();
        } else {
            this->state = new State("/Users/huwenjing/project02/CarrotFantasy/MapConfig/config1");
            this->state->show();
            this->state->play();
        }
    });
    connect(ui->setMapButton, &QPushButton::clicked, this, [=](){
        if (mapPath != "") {
            QMessageBox::StandardButton res = QMessageBox::question(this, "MapQuestion", "您已经设置过地图，是否重新设置地图？",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (res == QMessageBox::Yes) {
                QString path = QFileDialog::getOpenFileName(this, "打开文件", "/Users/huwenjing/project02/CarrotFantasy/MapConfig");
                if (path != "") {
                    this->state = new State(path);
                    mapPath = path;
                }
            }
        } else {
            QString path = QFileDialog::getOpenFileName(this, "打开文件", "/Users/huwenjing/project02/CarrotFantasy/MapConfig");
            if (path != "") {
                this->state = new State(path);
                mapPath = path;
            }
        }
    });
    connect(ui->getMapButton, &QPushButton::clicked, this, [=](){
        QMessageBox::information(this, "MapInformation", "地图成功导出！");
        if (mapPath == "") {
            QFile res("/Users/huwenjing/project02/CarrotFantasy/MapConfig/MapExport.txt");
            res.open(QIODevice::WriteOnly);
            QFile file("/Users/huwenjing/project02/CarrotFantasy/MapConfig/config1");
            file.open(QIODevice::ReadOnly);
            QByteArray read = file.readAll();
            res.write(read);
            res.close();
            file.close();
        } else {
            QFile res("/Users/huwenjing/project02/CarrotFantasy/MapConfig/MapExport.txt");
            res.open(QIODevice::WriteOnly);
            QFile file(mapPath);
            file.open(QIODevice::ReadOnly);
            QByteArray read = file.readAll();
            res.write(read);
            res.close();
            file.close();
        }
    });
}

