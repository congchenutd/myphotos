#ifndef BUTTONLINEEDIT_H
#define BUTTONLINEEDIT_H

#include <QWidget>
#include <QAbstractButton>
#include <QStyleOptionFrame>

class QLineEdit;

////////////////////////////////////////////////////////////////////////
/// A lineedit that allows embedding widgets (typically buttons) in it
/// There is always a lineedit in the middle
/// Users can add widgets to its left or right
/// Note: right widgets are added from right to left (i.e., the 1st added is the rightest)
class EmbeddableLineEdit : public QWidget
{
    Q_OBJECT

public:
    EmbeddableLineEdit(QWidget* parent = 0);

    QLineEdit* getLineEdit() const;
    void addLeftWidget (QWidget* widget);
    void addRightWidget(QWidget* widget);  // from right to left

    QSize sizeHint() const;
    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;

protected:
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void inputMethodEvent(QInputMethodEvent* e);
    bool event(QEvent* event);

protected:
    void updateGeometries();
    void initStyleOption(QStyleOptionFrame* option) const;

private:
    QLineEdit*      _lineEdit;
    QList<QWidget*> _leftWidgets;
    QList<QWidget*> _rightWidgets;
};


/////////////////////////////////////////////////////////////
/// A X button for SearchLineEdit
/// disappears (turns transparent) when text is empty
/// Does not hide for shortcut
///
class ClearButton : public QAbstractButton
{
    Q_OBJECT

public:
    ClearButton(QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void onTextChanged(const QString& text);

private:
    // a transparent button is invisible, but still accepts shortcut
    void setTransparent(bool transparent);

private:
    bool _transparent;
};

////////////////////////////////////////////////////////////////////////
/// A push button with just a pixmap
///
class PictureButton : public QAbstractButton
{
public:
    PictureButton(QWidget* parent = 0);
    void setPixmap(const QPixmap& pixmap);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QPixmap _pixmap;
};

#endif // BUTTONLINEEDIT_H
