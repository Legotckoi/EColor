#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T14:00:48
#
#-------------------------------------------------

QT       += core
QT       += gui

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wliteral-suffix

REVISION = $$system(git rev-parse HEAD)
DEFINES += APP_REVISION=$$REVISION

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EColor
TEMPLATE = app

RC_ICONS = myappico.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    dialogsettings.cpp

HEADERS  += mainwindow.h \
    about.h \
    settings.h \
    dialogsettings.h

FORMS    += mainwindow.ui \
    about.ui \
    dialogsettings.ui

RESOURCES += \
    images.qrc

