#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QLabel>

class QLineEdit;

class EditableLabel : public QLabel
{
    Q_OBJECT

public:
    EditableLabel();
    void edit();

protected:
    void resizeEvent(QResizeEvent* event);
    void mouseDoubleClickEvent(QMouseEvent*);

private slots:
    void onEditingFinished();

signals:
    void editingFinished(const QString&);

private:
    QLineEdit* _lineEdit;
};

#endif // EDITABLELABEL_H
