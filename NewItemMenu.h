#ifndef TAGMENU_H
#define TAGMENU_H

#include <QMenu>

/**
 * A menu with an item to create new items
 */
class NewItemMenu : public QMenu
{
    Q_OBJECT

public:
    NewItemMenu(const QString& newItemText, QWidget* parent = 0);

    void addAction(QAction* action);

private slots:
    void onNewItem();

signals:
    void newItemAdded(const QString&);

private:
    QString     _newItemText;
    QAction*    _separator;
};

#endif // TAGMENU_H
