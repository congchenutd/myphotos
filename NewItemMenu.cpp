#include "NewItemMenu.h"
#include <QAction>
#include <QInputDialog>

NewItemMenu::NewItemMenu(const QString& newItemText, QWidget* parent)
    : QMenu(parent),
      _newItemText(newItemText),
      _separator(0)
{
    _separator = addSeparator();

    QAction* actionNewItem = new QAction(_newItemText, this);
    connect(actionNewItem, SIGNAL(triggered(bool)), this, SLOT(onNewItem()));
    QMenu::addAction(actionNewItem);
}

void NewItemMenu::addAction(QAction* action) {
    insertAction(_separator, action);
}

void NewItemMenu::onNewItem()
{
    QString tag = QInputDialog::getText(this, _newItemText, _newItemText);
    if (!tag.isEmpty())
        emit newItemAdded(tag);
}
