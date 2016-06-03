#include "TagMenu.h"
#include <QAction>
#include <QInputDialog>

TagMenu::TagMenu(QWidget* parent)
    : QMenu(parent),
      _separator(0)
{
    setIcon(QIcon(":/Images/Tag.png"));
    setTitle("Tags");

    _separator = addSeparator();

    QAction* actionNewTag = new QAction(tr("New tag"), this);
    connect(actionNewTag, SIGNAL(triggered(bool)), this, SLOT(onNewTag()));
    QMenu::addAction(actionNewTag);
}

void TagMenu::addAction(QAction* action) {
    insertAction(_separator, action);
}

void TagMenu::onNewTag()
{
    QString tag = QInputDialog::getText(this, tr("New tag"), tr("Tag: "));
    if (!tag.isEmpty())
        emit newTag(tag);
}
