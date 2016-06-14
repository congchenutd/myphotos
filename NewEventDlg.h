#ifndef NEWEVENTDLG_H
#define NEWEVENTDLG_H

#include "NewMenuItemDlg.h"
#include "ui_NewEventDlg.h"

/**
 * Dialog for creating new event
 */
class NewEventDlg : public NewMenuItemDlg
{
public:
    explicit NewEventDlg(QWidget* parent = 0);

    QString getText() const;
    QDate   getDate() const;

private:
    Ui::NewEventDlg ui;
};

#endif // NEWEVENTDLG_H
