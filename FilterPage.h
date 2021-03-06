#ifndef FILTERPAGE_H
#define FILTERPAGE_H

#include <QWidget>

class ItemModel;

/**
 * A page on the sidebar for filtering photos by event, tag, or people
 */
class FilterPage : public QWidget
{
    Q_OBJECT

public:
    explicit FilterPage(QWidget* parent = 0);
    void setModel(ItemModel* model);
    ItemModel* getModel();
    virtual void update() = 0;

private:
    ItemModel* _model;
};

#endif // FILTERPAGE_H
