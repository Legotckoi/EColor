#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T14:00:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

linux {
    QT       += x11extras
    CONFIG   += link_pkgconfig
    PKGCONFIG += x11
}

CONFIG += c++11

win32-mingw{
    QMAKE_CXXFLAGS += -Wliteral-suffix
}
win32-msvc*{
    LIBS += -luser32
}

VER_MAJ = 0
VER_MIN = 1
VER_PAT = 5

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT} # major.minor.patch

VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"
DEFINES += VER_MAJOR=$${VER_MAJ}
DEFINES += VER_MINOR=$${VER_MIN}
DEFINES += VER_PATHES=$${VER_PAT}

TARGET = EColor
TEMPLATE = app

win32: RC_ICONS = images/myappico.ico

SOURCES += main.cpp\
    about.cpp \
    dialogsettings.cpp \
    versionchecker.cpp \
    dialogupdate.cpp \
    popupcolor.cpp \
    ecolorcore.cpp \
    popupmessage.cpp \
    transparentwindow.cpp \
    gradationlabel.cpp \
    popupcolorstylesheethelper.cpp

win32: SOURCES += win/qglobalshortcut.cpp
linux: SOURCES += x11/qglobalshortcut.cpp

HEADERS  += \
    about.h \
    settings.h \
    dialogsettings.h \
    versionchecker.h \
    dialogupdate.h \
    popupcolor.h \
    ecolorcore.h \
    popupmessage.h \
    transparentwindow.h \
    gradationlabel.h \
    popupcolorstylesheethelper.h \
    qglobalshortcut.h

FORMS    += \
    about.ui \
    dialogsettings.ui \
    dialogupdate.ui

RESOURCES += \
    images.qrc
