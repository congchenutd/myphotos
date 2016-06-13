#include "NewItemMenu.h"
#include "NewMenuItemDlg.h"
#include <QAction>
#include <QInputDialog>

NewItemMenu::NewItemMenu(const QString& newItemText, NewMenuItemDlg* dlg, QWidget* parent)
    : QMenu(parent),
      _newItemText(newItemText),
      _dlg(dlg)
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
    if (_dlg->exec() == QDialog::Accepted)
    {
        QString tag = _dlg->getText();
        if (!tag.isEmpty())
            emit newItemAdded(tag, _dlg->getDate(), _dlg->getImage());
    }

}
