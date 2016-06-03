#include "TagsViewlet.h"
#include "FlowLayout.h"

#include <QDrag>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QPalette>
#include <QDebug>

TagsViewlet::TagsViewlet(QWidget* parent)
    : QWidget(parent),
      _removable(false)
{
    _layout = new FlowLayout(this);
}

void TagsViewlet::loadTags(const QStringList& tags)
{
    _layout->clear();
    for (const QString& tag: tags)
        addTag(tag);
}

void TagsViewlet::addTag(const QString& tag)
{
    if (tagExists(tag))
        return;

    QLabel* label = new QLabel(tag, this);
    label->setFrameShape(QFrame::Panel);
    label->setFrameShadow(QFrame::Raised);
    QFont font;
    font.setPointSize(16);
    label->setFont(font);
    _layout->addWidget(label);
}

void TagsViewlet::removeTag(const QString& tag)
{
    for (int i = 0; i < layout()->count(); ++i)
    {
        QLabel* item = (QLabel*) layout()->itemAt(i)->widget();
        if (item->text() == tag)
            delete item;
    }
}

void TagsViewlet::setRemovable(bool removable) {
    _removable = removable;
}

bool TagsViewlet::tagExists(const QString& tag) const
{
    for (int i = 0; i < layout()->count(); ++i)
    {
        QLabel* item = (QLabel*) layout()->itemAt(i)->widget();
        if (item->text() == tag)
            return true;
    }
    return false;
}

void TagsViewlet::sort() {
    _layout->sort(TagLabelComparator());
}

bool TagLabelComparator::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const {
    return ((QLabel*) lhs->widget())->text() < ((QLabel*) rhs->widget())->text();
}
