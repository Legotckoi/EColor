#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T14:00:48
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32-mingw{
    QMAKE_CXXFLAGS += -std=c++0x
    QMAKE_CXXFLAGS += -Wliteral-suffix
}
win32-msvc*{
    LIBS += -luser32
}

VER_MAJ = 0
VER_MIN = 1
VER_PAT = 4

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT} # major.minor.patch

VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"
DEFINES += VER_MAJOR=$${VER_MAJ}
DEFINES += VER_MINOR=$${VER_MIN}
DEFINES += VER_PATHES=$${VER_PAT}



TARGET = EColor
TEMPLATE = app

RC_ICONS = images/myappico.ico

SOURCES += main.cpp\
    about.cpp \
    dialogsettings.cpp \
    versionchecker.cpp \
    dialogupdate.cpp \
    popupcolor.cpp \
    ecolorcore.cpp \
    popupmessage.cpp \
    codelabel.cpp \
    transparentwindow.cpp \
    gradationlabel.cpp

HEADERS  += \
    about.h \
    settings.h \
    dialogsettings.h \
    versionchecker.h \
    dialogupdate.h \
    popupcolor.h \
    ecolorcore.h \
    popupmessage.h \
    codelabel.h \
    transparentwindow.h \
    gradationlabel.h

FORMS    += \
    about.ui \
    dialogsettings.ui \
    dialogupdate.ui

RESOURCES += \
    images.qrc

