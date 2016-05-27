//#include "Event.h"
//#include "EventDAO.h"
//#include "Library.h"
//#include "People.h"
//#include "Photo.h"
//#include "PhotoDAO.h"
//#include "LibraryDAO.h"
//#include "Tag.h"

//#include <QObject>
//#include <QSqlDatabase>
//#include <QTest>
//#include <QMessageBox>

//bool openDB(const QString& name)
//{
//    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
//    database.setDatabaseName(name);
//    if(!database.open())
//    {
//        QMessageBox::critical(0, "Error", QObject::tr("Can not open database %1").arg(name));
//        return false;
//    }
//    return true;
//}

//class ModelTest : public QObject
//{
//    Q_OBJECT

//private slots:

//    void initTestCase()
//    {
//        openDB("MyPhotos.db");
//    }

//    void testLoad()
//    {
//        Library* library = Library::getInstance();
//        Event* event2 = new Event(2, "Event2", QDate::currentDate());
//        People* people2 = new People(2, "Cong");
//        Tag* tag2 = new Tag(2, "Play");

//        Photo* photo2 = new Photo(2, "Photo2", "/Users/Cong/OneDrive/Public/2016-05-21-甜豆100天-2.jpg",
//                                  QDateTime::currentDateTime());
//        photo2->setEvent(event2);
//        photo2->addPeople(people2);
//        photo2->addTag(tag2);

//        library->addTag(tag2);
//        library->addPeople(people2);
//        library->addEvent(event2);
//        library->addPhoto(photo2);
//    }

//    void cleanupTestCase()
//    {
//        Library::getInstance()->save();
//    }
//};


//QTEST_MAIN(ModelTest)
//#include "ModelTest.moc"
