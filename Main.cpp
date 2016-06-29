#include "MainWindow.h"
#include "SortableVBoxLayout.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QDir>
#include <QLabel>

// workaround for a bug on mac > Mavericks
// Finder returns / as the working path of an app bundle
// but if the app is run from terminal, the path is correct
// This method calcluates the path of the bundle from the application's path
QString getCurrentPath()
{
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    return dir.absolutePath();
}

bool openDB(const QString& name)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(name);
    if(!database.open())
    {
        QMessageBox::critical(0, "Error", database.lastError().text());
        return false;
    }
    return true;
}

struct Comparator
{
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const {
        QLabel* view1 = dynamic_cast<QLabel*>(lhs->widget());
        QLabel* view2 = dynamic_cast<QLabel*>(rhs->widget());
        if (view1 != 0 && view2 != 0)
            return view1->text() > view2->text();
        return false;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

#ifdef Q_OS_OSX
    QDir::setCurrent(getCurrentPath());
#endif

    if (!openDB("MyPhotos.db"))
        return 1;

    MainWindow window;
    window.showMaximized();

//    QWidget widget;
//    SortableVBoxLayout* layout = new SortableVBoxLayout(&widget);
//    layout->addWidget(new QLabel("a"));
//    layout->addWidget(new QLabel("b"));
//    layout->addWidget(new QLabel("c"));
//    layout->sort(Comparator());
//    widget.show();

    return app.exec();
}
