#ifndef SORTINGBAR_H
#define SORTINGBAR_H

#include <QToolBar>

class QRadioButton;

class ToolBar : public QToolBar
{
public:
    ToolBar(QWidget* parent = 0);

private:
    QRadioButton*  _radioName;
};

#endif // SORTINGBAR_H
