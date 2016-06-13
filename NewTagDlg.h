#ifndef NEWTAGDLG_H
#define NEWTAGDLG_H

#include "ui_NewTagDlg.h"
#include "NewMenuItemDlg.h"

class NewTagDlg : public NewMenuItemDlg
{
    Q_OBJECT

public:
    explicit NewTagDlg(QWidget* parent = 0);

    QString getText() const;

private:
    Ui::NewTagDlg ui;
};

#endif // NEWTAGDLG_H
