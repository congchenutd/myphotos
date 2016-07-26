#include "RenameDlg.h"

RenameDlg::RenameDlg(QWidget* parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}

QString RenameDlg::getTitle() const {
    return ui.lineEdit->text();
}
