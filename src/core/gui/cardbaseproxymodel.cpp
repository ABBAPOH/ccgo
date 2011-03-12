#include "cardbaseproxymodel.h"

#include <QTime>

class CardBaseProxyModelPrivate
{
public:
    QString nameFilter;
};

CardBaseProxyModel::CardBaseProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    d_ptr(new CardBaseProxyModelPrivate)
{
}

CardBaseProxyModel::~CardBaseProxyModel()
{
    delete d_ptr;
}

void CardBaseProxyModel::sort(int column, Qt::SortOrder order)
{
    QTime t;
    t.start();
    QSortFilterProxyModel::sort(column, order);
    qDebug("Sorting... time elapsed: %d ms - SortFilterProxyModel : sort", t.elapsed());
}

bool CardBaseProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 1, source_parent);
    return index.data().toString().startsWith(d_func()->nameFilter, filterCaseSensitivity());
}

void CardBaseProxyModel::setNameFilter(const QString name)
{
    Q_D(CardBaseProxyModel);
    d->nameFilter = name;
    invalidateFilter();
}
