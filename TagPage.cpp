#include "TagPage.h"
#include "ItemModel.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QButtonGroup>

TagPage::TagPage(QWidget* parent) :
    FilterPage(parent)
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

    connect(_btShowAll, SIGNAL(clicked()), SLOT(onShowAll()));
    connect(_radioAnd,  SIGNAL(clicked()), SLOT(onTagChecked()));
    connect(_radioOr,   SIGNAL(clicked()), SLOT(onTagChecked()));
}

void TagPage::update()
{
    QLayoutItem* item;
    while ((item = _layoutTags->takeAt(0)))
        delete item->widget();

    for (int i = 0; i < getModel()->rowCount(); ++i)
    {
        QString tag = getModel()->data(getModel()->index(i, 0)).toString();
        QCheckBox* checkBox = new QCheckBox(tag, this);
        connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(onTagChecked()));
        _layoutTags->addWidget(checkBox);
    }
    _layoutTags->addStretch(10);
}

void TagPage::onTagChecked()
{
    QStringList tags;
    for (int i = 0; i < _layoutTags->count(); ++i)
    {
        QLayoutItem* item = _layoutTags->itemAt(i);
        if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(item->widget()))
            if (checkBox->isChecked())
                tags << checkBox->text();
    }
    emit filter(tags, _radioAnd->isChecked());
}

void TagPage::onShowAll()
{
    for (int i = 0; i < _layoutTags->count(); ++i)
    {
        QLayoutItem* item = _layoutTags->itemAt(i);
        if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(item->widget()))
            checkBox->setChecked(false);
    }
    emit filter(QStringList(), _radioAnd->isChecked());
}

