#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T00:48:17
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyPhotos
TEMPLATE = app


SOURCES +=\
        MainWindow.cpp \
    Main.cpp \
    Library.cpp \
    Photo.cpp \
    Event.cpp \
    People.cpp \
    SettingsDlg.cpp \
    MonitoredFoldersDlg.cpp \
    Settings.cpp

HEADERS  += MainWindow.h \
    Library.h \
    Photo.h \
    Event.h \
    People.h \
    SettingsDlg.h \
    MonitoredFoldersDlg.h \
    Settings.h

FORMS    += MainWindow.ui \
    SettingsDlg.ui \
    MonitoredFoldersDlg.ui

RESOURCES += \
    Resources.qrc
