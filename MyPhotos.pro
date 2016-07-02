QT += core gui sql testlib widgets concurrent multimedia multimediawidgets positioning

TARGET = MyPhotos
TEMPLATE = app

#QMAKE_CXXFLAGS += -F/Users/Cong/Program/MyPhotos/VLC-Qt_1.0.1_osx/lib
#QMAKE_LFLAGS += -F/Users/Cong/Program/MyPhotos/VLC-Qt_1.0.1_osx/lib
#LIBS += -framework VLCQtCore

OPENCV_LOCATION = /usr/local/opt/opencv3
INCLUDEPATH += $$OPENCV_LOCATION/include

mac {
LIBS += -L$$OPENCV_LOCATION/lib \
	-lopencv_core \
	-lopencv_highgui \
	-lopencv_imgproc \
	-lopencv_video \
	-lopencv_videoio
}

win32 {
	RC_FILE = Resources.rc
}
macx {
	ICON = Images/Photos.icns
}

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
    TagDAO.cpp \
    PhotoView.cpp \
    FlowLayout.cpp \
    Thumbnail.cpp \
    ThumbnailDAO.cpp \
    PhotoItem.cpp \
    EditableLabel.cpp \
    NewItemMenu.cpp \
    Scanner.cpp \
    NewMenuItemDlg.cpp \
    NewTagDlg.cpp \
    NewEventDlg.cpp \
    EventPage.cpp \
    ItemModel.cpp \
    PeopleModel.cpp \
    FilterPage.cpp \
    EventModel.cpp \
    TagModel.cpp \
    TagPage.cpp \
    SliderWithToolTip.cpp \
    ThumbnailGenerator.cpp \
    InfoPage.cpp \
    InfoModel.cpp \
    Geocoder.cpp \
    Exif.cpp \
    Clustering.cpp \
    ClusterView.cpp \
    PhotoClusters.cpp \
    SortableVBoxLayout.cpp

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
    PhotoView.h \
    FlowLayout.h \
    Thumbnail.h \
    ThumbnailDAO.h \
    PhotoItem.h \
    EditableLabel.h \
    TagDAO.h \
    NewItemMenu.h \
    Scanner.h \
    NewMenuItemDlg.h \
    NewTagDlg.h \
    NewEventDlg.h \
    EventPage.h \
    ItemModel.h \
    PeopleModel.h \
    FilterPage.h \
    EventModel.h \
    TagModel.h \
    TagPage.h \
    SliderWithToolTip.h \
    ThumbnailGenerator.h \
    InfoPage.h \
    InfoModel.h \
    Geocoder.h \
    Exif.h \
    Clustering.h \
    ClusterView.h \
    PhotoClusters.h \
    SortableVBoxLayout.h \
    QuickSort.h

FORMS += \
	MainWindow.ui \
    SettingsDlg.ui \
    MonitoredFoldersDlg.ui \
    TagsView.ui \
    NewTagDlg.ui \
    NewEventDlg.ui \
    EventPage.ui \
    InfoPage.ui

RESOURCES += \
    Resources.qrc

DISTFILES +=
