#include "EditableLabel.h"

#include <QLineEdit>

EditableLabel::EditableLabel()
{
    _lineEdit = new QLineEdit(this);
    _lineEdit->hide();
    connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
}

void EditableLabel::edit()
{
    _lineEdit->show();
    _lineEdit->setText(this->text());
    _lineEdit->selectAll();
    _lineEdit->setFocus();
}

void EditableLabel::resizeEvent(QResizeEvent* event)
{
    _lineEdit->resize(width(), height());
    QLabel::resizeEvent(event);
}

void EditableLabel::onEditingFinished()
{
    _lineEdit->hide();
    setText(_lineEdit->text());
    emit editingFinished(_lineEdit->text());
}

