#include "EmbeddableLineEdit.h"
#include <QFocusEvent>
#include <QLineEdit>
#include <QPainter>
#include <QStyle>

EmbeddableLineEdit::EmbeddableLineEdit(QWidget* parent)
    : QWidget(parent),
      _lineEdit(new QLineEdit(this))
{
    setFocusPolicy(_lineEdit->focusPolicy());
    setAttribute(Qt::WA_InputMethodEnabled);
    setSizePolicy(_lineEdit->sizePolicy());
    setBackgroundRole(_lineEdit->backgroundRole());
    setMouseTracking(true);
    setAcceptDrops(true);
    setAttribute(Qt::WA_MacShowFocusRect, true);
    QPalette p = _lineEdit->palette();
    setPalette(p);

    // line edit
    _lineEdit->setFrame(false);
    _lineEdit->setFocusProxy(this);
    QPalette clearPalette = _lineEdit->palette();
    clearPalette.setBrush(QPalette::Base, QBrush(Qt::transparent));
    _lineEdit->setPalette(clearPalette);
}

QLineEdit* EmbeddableLineEdit::getLineEdit() const {
    return _lineEdit;
}

void EmbeddableLineEdit::addLeftWidget(QWidget* widget)
{
    if(widget != 0)
        _leftWidgets << widget;
}

void EmbeddableLineEdit::addRightWidget(QWidget* widget)
{
    if(widget != 0)
        _rightWidgets << widget;
}

void EmbeddableLineEdit::resizeEvent(QResizeEvent* event)
{
    updateGeometries();
    QWidget::resizeEvent(event);
}

void EmbeddableLineEdit::updateGeometries()
{
    QStyleOptionFrame panel;
    initStyleOption(&panel);
    QRect rect = style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);

    int hSpace = 2;

    // left widgets are placed from left to right
    int xLeft = rect.x() + hSpace;
    foreach(QWidget* widget, _leftWidgets)
    {
        widget->setGeometry(xLeft,
                            rect.y() + (rect.height() - widget->height())/2,
                            widget->width(),
                            widget->height());
        xLeft+= widget->width() + hSpace;
    }

    // right widgets are placed from right to left
    int xRight = rect.right() - hSpace;
    foreach(QWidget* widget, _rightWidgets)
    {
        widget->setGeometry(xRight - widget->width(),
                            rect.y() + (rect.height() - widget->height())/2,
                            widget->width(),
                            widget->height());
        xRight -= widget->width() + hSpace;
    }

    // lineedit is placed in the middle
    _lineEdit->setGeometry(xLeft, 0, xRight - xLeft, height());
}

void EmbeddableLineEdit::initStyleOption(QStyleOptionFrame* option) const
{
    option->initFrom(this);
    option->rect = contentsRect();
    option->lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this);
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;
    if (_lineEdit->isReadOnly())
        option->state |= QStyle::State_ReadOnly;
    option->features = QStyleOptionFrame::None;
}

QSize EmbeddableLineEdit::sizeHint() const
{
    _lineEdit->setFrame(true);
    QSize size = _lineEdit->sizeHint();
    _lineEdit->setFrame(false);
    return size;
}

void EmbeddableLineEdit::focusInEvent(QFocusEvent *event)
{
    _lineEdit->event(event);
    QWidget::focusInEvent(event);
}

void EmbeddableLineEdit::focusOutEvent(QFocusEvent *event)
{
    _lineEdit->event(event);
    QWidget::focusOutEvent(event);
}

void EmbeddableLineEdit::keyPressEvent(QKeyEvent* event) {
    _lineEdit->event(event);
}

bool EmbeddableLineEdit::event(QEvent* event)
{
    if (event->type() == QEvent::ShortcutOverride)
        return _lineEdit->event(event);
    return QWidget::event(event);
}

void EmbeddableLineEdit::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    QStyleOptionFrame panel;
    initStyleOption(&panel);
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &p, this);
}

QVariant EmbeddableLineEdit::inputMethodQuery(Qt::InputMethodQuery property) const {
    return _lineEdit->inputMethodQuery(property);
}

void EmbeddableLineEdit::inputMethodEvent(QInputMethodEvent* e) {
    _lineEdit->event(e);
}


//////////////////////////////////////////////////////////////////////
ClearButton::ClearButton(QWidget* parent) : QAbstractButton(parent)
{
    setCursor(Qt::ArrowCursor);
    setFocusPolicy(Qt::NoFocus);
    setTransparent(true);
}

void ClearButton::paintEvent(QPaintEvent*)
{
    if(_transparent)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int margin = 4;
    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawLine(margin, margin, width() - margin, height() - margin);
    painter.drawLine(margin, height() - margin, width() - margin, margin);
}

void ClearButton::onTextChanged(const QString& text) {
    setTransparent(text.isEmpty());
}

void ClearButton::setTransparent(bool transparent)
{
    _transparent = transparent;
    repaint();
}


//////////////////////////////////////////////////////////////////////
PictureButton::PictureButton(QWidget* parent) : QAbstractButton(parent)
{
    setCursor(Qt::ArrowCursor);
    setFocusPolicy(Qt::NoFocus);
}

void PictureButton::setPixmap(const QPixmap& pixmap) {
    _pixmap = pixmap;
}

void PictureButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    int margin = 1;
    int offset = isDown() ? 2 : 0;
    painter.drawPixmap(offset + margin,  offset + margin,
                       width() - margin, height() - margin,
                       _pixmap);
}
