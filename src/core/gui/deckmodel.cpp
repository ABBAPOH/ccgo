#include "deckmodel.h"
#include "deckmodel_p.h"

DeckModelPrivate::DeckModelPrivate() :
    rootItem(new TreeItem),
    deck(0)
{
    keys << "Name" << "id" << "Edition" << "Edition Name" << "Color" << "Type"
                      << "Rarity" << "Cost" << "Text" << "CMC" << "Artist" << "Legal"
                      << "Flavor" << "No" << "Power" << "Toughness";

}

DeckModelPrivate::~DeckModelPrivate()
{
    delete rootItem;
}

void DeckModelPrivate::buildTree()
{
    if (!rootItem) {
        delete rootItem;
    }
    rootItem = new TreeItem();

    foreach (QString group, deck->groups()) {
        TreeItem *groupItem = new TreeItem(rootItem);
        groupItem->group = group;

        foreach (Card c, deck->cards(group)) {
            TreeItem *child = findItem(groupItem, c);

            if (!child) {
                child = createItem(groupItem, c);
            } else {
                child->count++;
            }
        }
    }
}

TreeItem *DeckModelPrivate::createItem(TreeItem *groupItem, const Card &card)
{
    TreeItem *item = new TreeItem(groupItem);
    item->card = card;
    item->count = 1;
    return item;
}

TreeItem * DeckModelPrivate::findItem(const QString &group)
{
    foreach (TreeItem *item, rootItem->m_children) {
        if (item->group == group)
            return item;
    }
    return 0;
}

TreeItem * DeckModelPrivate::findItem(TreeItem *groupItem, const Card &c)
{
    if (!groupItem)
        return 0;

    foreach (TreeItem *item, groupItem->m_children) {
        if (item->card == c) {
            return item;
        }
    }
    return 0;
}

DeckModel::DeckModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new DeckModelPrivate)
{
}

DeckModel::DeckModel(Deck *deck, QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new DeckModelPrivate)
{
    setDeck(deck);
}

DeckModel::~DeckModel()
{
    delete d_ptr;
}

int DeckModel::columnCount(const QModelIndex &parent) const
{
    return 16; // TODO: take from game
}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{
    Q_D(const DeckModel);

    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item->parent() == d->rootItem) { // it is gruop item
            if (index.column() == 0)
                return item->group;
            else
                return "";
        } else {
            int column = index.column();
            if (column == 1)
                return item->count;
            else
                return item->card.attribute(d->keys[column == 0 ? 0 : column - 1]);
        }
        return item->row();
    }

    return QVariant();
}

Qt::ItemFlags DeckModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DeckModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_D(const DeckModel);

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 1)
            return "Count";
        return d->keys[section == 0 ? 0 : section - 1];
    }

    return QVariant();
}

QModelIndex DeckModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const DeckModel);

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DeckModel::parent(const QModelIndex &index) const
{
    Q_D(const DeckModel);

    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == d->rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DeckModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const DeckModel);

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = d->rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

Deck * DeckModel::deck()
{
    Q_D(DeckModel);

    return d->deck;
}

void DeckModel::setDeck(Deck *deck)
{
    Q_D(DeckModel);

    if (d->deck) {
        disconnect(d->deck, SIGNAL(cardAdded(QString,Card)), this, SLOT(addCard(QString,Card)));
        disconnect(d->deck, SIGNAL(cardRemoved(QString,Card)), this, SLOT(removeCard(QString,Card)));
    }

    connect(deck, SIGNAL(cardAdded(QString,Card)), SLOT(addCard(QString,Card)));
    connect(deck, SIGNAL(cardRemoved(QString,Card)), SLOT(removeCard(QString,Card)));
    d->deck = deck;
    d->buildTree();
}

void DeckModel::addCard(const QString &group, const Card &card)
{
    Q_D(DeckModel);

    if (sender() == d->deck) {
        TreeItem *groupItem = d->findItem(group);
        if (!groupItem) {
            return;
        }

        TreeItem *item = d->findItem(groupItem, card);
        if (!item) {
            QModelIndex parent = index(groupItem->row(), 0);
            beginInsertRows(parent, groupItem->childCount(), groupItem->childCount());
            item = d->createItem(groupItem, card);
            endInsertRows();
        } else {
            QModelIndex parent = index(groupItem->row(), 0);
            QModelIndex child = index(item->row(), 1, parent);
            item->count++;
            emit dataChanged(child, child);
        }
    } else {
        d->deck->addCard(group, card);
    }
}

void DeckModel::removeCard(const QString &group, const Card &card)
{
    Q_D(DeckModel);

    if (sender() == d->deck) {
        TreeItem *groupItem = d->findItem(group);
        if (!groupItem) {
            return;
        }

        TreeItem *item = d->findItem(groupItem, card);
        if (item) {
            if (item->count == 1) {
                QModelIndex parent = index(groupItem->row(), 0);
                beginRemoveRows(parent, item->row(), item->row());
                delete item;
                endRemoveRows();
            } else {
                QModelIndex parent = index(groupItem->row(), 0);
                QModelIndex child = index(item->row(), 1, parent);
                item->count--;
                emit dataChanged(child, child);
            }
        } else {
            return;
        }
    } else {
        d->deck->removeCard(group, card);
    }
}

void DeckModel::removeCard(const QModelIndex &index)
{
    Q_D(DeckModel);

    if (!index.isValid()) {
        return;
    }

    TreeItem *groupItem = static_cast<TreeItem *>(index.parent().internalPointer());
    if (groupItem == d->rootItem)
        return;

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    removeCard(groupItem->group, item->card);
}


