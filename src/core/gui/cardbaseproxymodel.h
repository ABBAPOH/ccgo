#ifndef CARDBASEPROXYMODEL_H
#define CARDBASEPROXYMODEL_H

#include <QtGui/QSortFilterProxyModel>

class CardBaseProxyModelPrivate;
class CardBaseProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CardBaseProxyModel)
public:
    explicit CardBaseProxyModel(QObject *parent = 0);
    ~CardBaseProxyModel();

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    void setNameFilter(const QString name);

signals:

public slots:

protected:
    CardBaseProxyModelPrivate *d_ptr;
};

#endif // CARDBASEPROXYMODEL_H
