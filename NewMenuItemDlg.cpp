#include "NewMenuItemDlg.h"

NewMenuItemDlg::NewMenuItemDlg(QWidget* parent)
    : QDialog(parent)
{}

QString NewMenuItemDlg::getText()   const { return QString(); }
QDate   NewMenuItemDlg::getDate()   const { return QDate();   }
QString NewMenuItemDlg::getImage()  const { return QString(); }
