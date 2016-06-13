#ifndef TAGMENU_H
#define TAGMENU_H

#include <QMenu>

class NewMenuItemDlg;

/**
 * A menu with an item to create new items
 */
class NewItemMenu : public QMenu
{
    Q_OBJECT

public:
    NewItemMenu(const QString& newItemText, NewMenuItemDlg* dlg, QWidget* parent = 0);

    void addAction(QAction* action);

private slots:
    void onNewItem();

signals:
    void newItemAdded(const QString& text, const QDate& date, const QString& image);

private:
    QString     _newItemText;
    QAction*    _separator;
    NewMenuItemDlg* _dlg;
};

#endif // TAGMENU_H
