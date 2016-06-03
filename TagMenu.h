#ifndef TAGMENU_H
#define TAGMENU_H

#include <QMenu>

class TagMenu : public QMenu
{
    Q_OBJECT

public:
    TagMenu(QWidget* parent = 0);

    void addAction(QAction* action);

private slots:
    void onNewTag();

signals:
    void newTag(const QString&);

private:
    QAction* _separator;
};

#endif // TAGMENU_H
