#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T00:48:17
#
#-------------------------------------------------

QT += core gui sql testlib widgets

TARGET = MyPhotos
TEMPLATE = app

SOURCES += \
	MainWindow.cpp \
    Main.cpp \
    Library.cpp \
    Photo.cpp \
    Event.cpp \
    People.cpp \
    SettingsDlg.cpp \
    MonitoredFoldersDlg.cpp \
    Settings.cpp \
    LibraryDAO.cpp \
    PeopleDAO.cpp \
    EventDAO.cpp \
    PhotoDAO.cpp \
    ModelTest.cpp \
    Persistable.cpp \
    DAO.cpp \
    Tag.cpp \
    TagDAO.cpp

HEADERS += \
	MainWindow.h \
    Library.h \
    Photo.h \
    Event.h \
    People.h \
    SettingsDlg.h \
    MonitoredFoldersDlg.h \
    Settings.h \
    LibraryDAO.h \
    PeopleDAO.h \
    EventDAO.h \
    Persistable.h \
    DAO.h \
    PhotoDAO.h \
    Tag.h \
    TagDAO.h

FORMS += \
	MainWindow.ui \
    SettingsDlg.ui \
    MonitoredFoldersDlg.ui

RESOURCES += \
    Resources.qrc
