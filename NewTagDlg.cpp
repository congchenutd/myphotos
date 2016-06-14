#include "NewTagDlg.h"

NewTagDlg::NewTagDlg(const QString &title, QWidget* parent) :
    NewMenuItemDlg(parent)
{
    ui.setupUi(this);
    setWindowTitle(title);
}

QString NewTagDlg::getText() const {
    return ui.leTagName->text();
}
