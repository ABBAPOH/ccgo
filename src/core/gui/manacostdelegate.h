#ifndef MANACOSTDELEGATE_H
#define MANACOSTDELEGATE_H

#include <QtGui/QStyledItemDelegate>

class ManaCostDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ManaCostDelegate(QObject *parent = 0);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

private:
    QMap<QString, QPixmap> pixmaps;
};

#endif // MANACOSTDELEGATE_H
