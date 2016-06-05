#include "TagsView.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QButtonGroup>

TagsView::TagsView(QWidget* parent) :
    QWidget(parent)
{
    _layoutTop = new QVBoxLayout(this);

    _layoutAnd = new QHBoxLayout;
    _radioAnd = new QRadioButton("And", this);
    _radioOr  = new QRadioButton("Or",  this);
    _radioOr->setChecked(true);
    QButtonGroup* buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(_radioAnd);
    buttonGroup->addButton(_radioOr);
    _layoutAnd->addWidget(_radioAnd);
    _layoutAnd->addWidget(_radioOr);

    _btShowAll = new QPushButton(tr("Show all"), this);
    _layoutTop->addWidget(_btShowAll);

    _layoutTags = new QVBoxLayout;
    _layoutTop->addLayout(_layoutAnd);
    _layoutTop->addLayout(_layoutTags);

    connect(_btShowAll, SIGNAL(clicked()), this, SLOT(onShowAll()));
    connect(_radioAnd,  SIGNAL(clicked()), this, SLOT(onTagChecked()));
    connect(_radioOr,   SIGNAL(clicked()), this, SLOT(onTagChecked()));
}

void TagsView::setTags(const QStringList& tags)
{
    QLayoutItem* item;
    while ((item = _layoutTags->takeAt(0)))
        delete item->widget();

    foreach (const QString& tag, tags)
    {
        QCheckBox* checkBox = new QCheckBox(tag, this);
        connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(onTagChecked()));
        _layoutTags->addWidget(checkBox);
    }
    _layoutTags->addStretch(10);
}

void TagsView::onTagChecked()
{
    QStringList tags;
    for (int i = 0; i < _layoutTags->count(); ++i)
    {
        QLayoutItem* item = _layoutTags->itemAt(i);
        if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(item->widget()))
            if (checkBox->isChecked())
                tags << checkBox->text();
    }
    emit filterByTags(tags, _radioAnd->isChecked());
}

void TagsView::onShowAll()
{
    for (int i = 0; i < _layoutTags->count(); ++i)
    {
        QLayoutItem* item = _layoutTags->itemAt(i);
        if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(item->widget()))
            checkBox->setChecked(false);
    }
    emit filterByTags(QStringList(), _radioAnd->isChecked());
}

