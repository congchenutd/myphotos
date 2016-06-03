#ifndef TAGSVIEW_H
#define TAGSVIEW_H

#include "ui_TagsView.h"

class TagsView : public QWidget
{
    Q_OBJECT

public:
    explicit TagsView(QWidget* parent = 0);

private:
    Ui::TagsView ui;
};

#endif // TAGSVIEW_H
