#ifndef NEWTAGDLG_H
#define NEWTAGDLG_H

#include "ui_NewTagDlg.h"
#include "NewMenuItemDlg.h"

/**
 * Dialog for creating new tag/people
 */
class NewTagDlg : public NewMenuItemDlg
{
public:
    explicit NewTagDlg(const QString& title, QWidget* parent = 0);

    QString getText() const;

private:
    Ui::NewTagDlg ui;
};

#endif // NEWTAGDLG_H
