#ifndef SLIDERWITHTOOLTIP_H
#define SLIDERWITHTOOLTIP_H

#include <QSlider>

/**
 * A slider that shows its value using a tooltip
 */
class SliderWithToolTip : public QSlider
{
public:
    SliderWithToolTip(QWidget* parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent* event);
};

#endif // SLIDERWITHTOOLTIP_H
