#include "SliderWithToolTip.h"

#include <QMouseEvent>
#include <QToolTip>

SliderWithToolTip::SliderWithToolTip(QWidget* parent)
    : QSlider(parent)
{
}

void SliderWithToolTip::mouseMoveEvent(QMouseEvent* event)
{
    QToolTip::showText(event->globalPos(), tr("%1").arg(value()));
    QSlider::mouseMoveEvent(event);
}
