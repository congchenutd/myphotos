#include "TagsView.h"
#include <QInputDialog>
#include <QLineEdit>

TagsView::TagsView(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    ui.thisPhoto->setRemovable(true);
    ui.available->setRemovable(false);
    connect(ui.btNewTag, SIGNAL(clicked(bool)), this, SLOT(onNewTag()));
}

void TagsView::onNewTag()
{
    QString tag = QInputDialog::getText(this, tr("New tag"), tr("Tag:"));
    if (!tag.isEmpty())
    {
        ui.available->addTag(tag);
        ui.thisPhoto->addTag(tag);
    }
}
