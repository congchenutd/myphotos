#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QLabel>

class QLineEdit;

/**
 * A Label that can be edited by double click
 * A QLineEdit is used as the editor
 */
class EditableLabel : public QLabel
{
    Q_OBJECT

public:
    EditableLabel();
    void edit();
    void finishEditing();
    void cancelEditing();

protected:
    void resizeEvent            (QResizeEvent* event);
    void mouseDoubleClickEvent  (QMouseEvent*);
    void keyPressEvent          (QKeyEvent* event);
    bool eventFilter            (QObject* obj, QEvent* event);

signals:
    void editingFinished(const QString&);

private:
    QLineEdit* _lineEdit;
};

#endif // EDITABLELABEL_H
