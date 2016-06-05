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
VER_PAT = 7

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT} # major.minor.patch

VERSTR = '\\"$${VERSION}\\"'
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += VER=\"$${VERSTR}\"
DEFINES += VER_MAJOR=$${VER_MAJ}
DEFINES += VER_MINOR=$${VER_MIN}
DEFINES += VER_PATHES=$${VER_PAT}

TARGET = EColor
TEMPLATE = app
CONFIG += warn_on

DESTDIR = ../bin
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
UI_DIR = ../build/ui
unix:OBJECTS_DIR = ../build/o/unix
win32:OBJECTS_DIR = ../build/o/win32
macx:OBJECTS_DIR = ../build/o/mac

win32: RC_ICONS = $$PWD/images/myappico.ico

TRANSLATIONS += $$PWD/translations/ecolor_ru.ts

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
    popupcolorstylesheethelper.h

include(QGlobalShortcut/qglobalshortcut.pri)

FORMS    += \
    about.ui \
    dialogsettings.ui \
    dialogupdate.ui

RESOURCES += \
    images.qrc \
    translations.qrc

DISTFILES +=
