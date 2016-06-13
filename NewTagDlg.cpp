#include "NewTagDlg.h"

NewTagDlg::NewTagDlg(QWidget* parent) :
    NewMenuItemDlg(parent)
{
    ui.setupUi(this);
}

QString NewTagDlg::getText() const {
    return ui.leTagName->text();
}
