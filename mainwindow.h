#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QPalette>
#include <QBrush>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "state.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setButton();
    void paintEvent(QPaintEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    State *state;
    QString mapPath;
};

#endif // MAINWINDOW_H

