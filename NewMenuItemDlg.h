#ifndef NEWMENUITEMDLG_H
#define NEWMENUITEMDLG_H

#include <QDate>
#include <QDialog>

class NewMenuItemDlg : public QDialog
{
public:
    NewMenuItemDlg(QWidget* parent);

    virtual QString getText()   const;
    virtual QDate   getDate()   const;
    virtual QString getImage()  const;
};

#endif // NEWMENUITEMDLG_H
