#ifndef LIBRARYDAO_H
#define LIBRARYDAO_H

#include <QMap>
#include <QObject>

class Library;

class LibraryDAO: public QObject
{
public:
    static LibraryDAO* getInstance();
    void load(Library* library);
    void save(Library* library);
    void clean();

private:
    void removeUnusedTags();

private:
    LibraryDAO();
    void createTables();
};

#endif // LIBRARYDAO_H
