#-------------------------------------------------
#
# Project created by QtCreator 2020-10-19T14:45:22
#
#-------------------------------------------------

QT       += core gui network
QT       += quickwidgets
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-3dprint
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS XH_WIN XH_VIS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    askpause.cpp \
    selectmode.cpp \
    choosefile.cpp \
    parsing.cpp \
    delete.cpp \
    mylistwidgetitem.cpp \
    xhwizard.cpp \
    xhrest.cpp \
    xhsensor.cpp \
    xhpage.cpp \
    xhport.cpp \
    filecheckthread.cpp \
    choosetemp.cpp \
    unnoknfile.cpp \
    powerlost.cpp \
    filamentfault.cpp \
    printflament.cpp \
    XhRoundProgressBar.cpp \
    XhRoundProgressBarTwo.cpp \
    printsetdlog.cpp \
    parsetdlog.cpp \
    dupandmirorr.cpp \
    duponly.cpp \
    mywifiitem.cpp \
    updateprogrebar.cpp \
    XhUpdater.cpp \
    XhGcodeFileParser.cpp \
    xhcontrolr818.cpp

HEADERS += \
        mainwindow.h \
    askpause.h \
    selectmode.h \
    choosefile.h \
    parsing.h \
    delete.h \
    mylistwidgetitem.h \
    xhwizard.h \
    xhrest.h \
    xhsensor.h \
    xhpage.h \
    xhport.h \
    filecheckthread.h \
    choosetemp.h \
    unnoknfile.h \
    powerlost.h \
    filamentfault.h \
    printflament.h \
    XhRoundProgressBar.h \
    XhRoundProgressBarTwo.h \
    printsetdlog.h \
    parsetdlog.h \
    dupandmirorr.h \
    duponly.h \
    mywifiitem.h \
    updateprogrebar.h \
    XhUpdater.h \
    XhGcodeFileParser.h \
    xhcontrolr818.h

FORMS += \
        mainwindow.ui \
    askpause.ui \
    selectmode.ui \
    choosefile.ui \
    parsing.ui \
    delete.ui \
    mylistwidgetitem.ui \
    xhwizard.ui \
    xhrest.ui \
    xhsensor.ui \
    choosetemp.ui \
    unnoknfile.ui \
    powerlost.ui \
    filamentfault.ui \
    printflament.ui \
    printsetdlog.ui \
    parsetdlog.ui \
    dupandmirorr.ui \
    duponly.ui \
    mywifiitem.ui \
    updateprogrebar.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    qml.qrc
