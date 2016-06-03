#ifndef TAGSVIEWLET_H
#define TAGSVIEWLET_H

#include <QLabel>
#include <QWidget>
#include <functional>

class FlowLayout;
class QLayoutItem;

class TagsViewlet : public QWidget
{
public:
    explicit TagsViewlet(QWidget* parent = 0);
    void loadTags   (const QStringList& tags);
    void addTag     (const QString& tag);
    void removeTag  (const QString& tag);
    void setRemovable(bool removable);
    bool tagExists(const QString& tag) const;

protected:

private:
    void sort();

private:
    FlowLayout* _layout;
    bool        _removable;
};

struct TagLabelComparator {
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
};

#endif // TAGSVIEWLET_H
