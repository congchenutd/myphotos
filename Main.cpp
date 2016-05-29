#include "MainWindow.h"
#include <QSqlDatabase>
#include <QMessageBox>

bool openDB(const QString& name)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(name);
    if(!database.open())
    {
        QMessageBox::critical(0, "Error", QObject::tr("Can not open database %1").arg(name));
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (!openDB("MyPhotos.db"))
        return 1;

    QApplication app(argc, argv);
    MainWindow window;
    window.showMaximized();

    return app.exec();
}
