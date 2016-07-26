#ifndef RENAMEDLG_H
#define RENAMEDLG_H

#include "ui_RenameDlg.h"

class RenameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDlg(QWidget* parent = 0);
    QString getTitle() const;

private:
    Ui::RenameDlg ui;
};

#endif // RENAMEDLG_H
