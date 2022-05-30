QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aggressivefly.cpp \
    attacklabel.cpp \
    bee.cpp \
    bodyguardant.cpp \
    enemy.cpp \
    fireant.cpp \
    harvesterant.cpp \
    lattice.cpp \
    main.cpp \
    mainwindow.cpp \
    mapconfig.cpp \
    ninjaant.cpp \
    queenant.cpp \
    scaryfly.cpp \
    scubaant.cpp \
    slowant.cpp \
    state.cpp \
    sun.cpp \
    tankant.cpp \
    throwerant.cpp \
    tower.cpp \
    wallant.cpp

HEADERS += \
    aggressivefly.h \
    attacklabel.h \
    bee.h \
    bodyguardant.h \
    enemy.h \
    fireant.h \
    harvesterant.h \
    lattice.h \
    mainwindow.h \
    mapconfig.h \
    ninjaant.h \
    queenant.h \
    scaryfly.h \
    scubaant.h \
    slowant.h \
    state.h \
    sun.h \
    tankant.h \
    throwerant.h \
    tower.h \
    wallant.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets/assets.qrc
