#include "EditableLabel.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QEvent>

EditableLabel::EditableLabel()
{
    _lineEdit = new QLineEdit(this);
    _lineEdit->hide();
    _lineEdit->installEventFilter(this);
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
    _lineEdit->resize(size());
    QLabel::resizeEvent(event);
}

void EditableLabel::mouseDoubleClickEvent(QMouseEvent*) {
    edit();
}

void EditableLabel::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter ||
        event->key() == Qt::Key_Return)
        finishEditing();
    else if (event->key() == Qt::Key_Escape)
        cancelEditing();
}

bool EditableLabel::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::FocusOut)
    {
        finishEditing();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void EditableLabel::finishEditing()
{
    setText(_lineEdit->text());
    _lineEdit->hide();
    emit editingFinished(_lineEdit->text());
}

void EditableLabel::cancelEditing() {
    _lineEdit->setText(text());
    _lineEdit->hide();
}

