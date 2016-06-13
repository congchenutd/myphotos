#include "FilterPage.h"

FilterPage::FilterPage(QWidget* parent) : QWidget(parent)
{}

void FilterPage::setModel(ItemModel* model)
{
    _model = model;
    update();
}

ItemModel* FilterPage::getModel() {
    return _model;
}
