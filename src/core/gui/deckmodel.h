#ifndef DECKMODEL_H
#define DECKMODEL_H

#include <QtCore/QAbstractItemModel>

class Card;
class Deck;
class DeckModelPrivate;
class DeckModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DeckModel)
public:
    explicit DeckModel(QObject *parent = 0);
    DeckModel(Deck *deck, QObject *parent = 0);
    ~DeckModel();

    Deck *deck();
    void setDeck(Deck *deck);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex topGroupIndex(const QModelIndex &index);

signals:

public slots:

protected:
    DeckModelPrivate *d_ptr;
};

#endif // DECKMODEL_H
