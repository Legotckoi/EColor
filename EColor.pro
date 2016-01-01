#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T14:00:48
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += network

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wliteral-suffix

VER_MAJ = 0
VER_MIN = 1
VER_PAT = 0

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT} # major.minor.patch

VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"
DEFINES += VER_MAJOR=$${VER_MAJ}
DEFINES += VER_MINOR=$${VER_MIN}
DEFINES += VER_PATHES=$${VER_PAT}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EColor
TEMPLATE = app

RC_ICONS = myappico.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    dialogsettings.cpp \
    versionchecker.cpp \
    dialogupdate.cpp

HEADERS  += mainwindow.h \
    about.h \
    settings.h \
    dialogsettings.h \
    versionchecker.h \
    dialogupdate.h

FORMS    += mainwindow.ui \
    about.ui \
    dialogsettings.ui \
    dialogupdate.ui

RESOURCES += \
    images.qrc

