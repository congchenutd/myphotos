#include "NewEventDlg.h"

NewEventDlg::NewEventDlg(QWidget* parent) :
    NewMenuItemDlg(parent)
{
    ui.setupUi(this);
    ui.dateEdit->setDate(QDate::currentDate());
}

QString NewEventDlg::getText() const {
    return ui.leEventName->text();
}

QDate NewEventDlg::getDate() const {
    return ui.dateEdit->date();
}
