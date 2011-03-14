#include "deckmodel.h"
#include "deckmodel_p.h"

QString getCardType(const Card &card)
{
    // FIXME: hardcoded
    QString type = card.attribute("Type").toString();
    if (type.contains("Land")) {
        return "Lands";
    }
    if (type.contains("Creature")) {
        return "Creatures";
    }
    if (type.contains("Artifact")) {
        return "Artifacts";
    }
    if (type.contains("Enchantment")) {
        return "Enchantments";
    }
    if (type.contains("Instant")) {
        return "Spells";
    }
    if (type.contains("Sorcery")) {
        return "Spells";
    }
    if (type.contains("Planeswalker")) {
        return "Planeswalkers";
    }

    return "Other";
}

DeckModelPrivate::DeckModelPrivate(DeckModel *qq) :
    q_ptr(qq),
    rootItem(new TreeItem),
    deck(0)
{
}

DeckModelPrivate::~DeckModelPrivate()
{
    delete rootItem;
}

void DeckModelPrivate::buildTree()
{
    foreach (QString group, deck->groups()) {
        foreach (Card card, deck->cards(group)) {
            onCardAdded(card, group);
            int count = deck->count(card);
            onCountChanged(card, group, count);
        }
    }
}

void DeckModelPrivate::updateItemCount(TreeItem *item, int count)
{
    Q_Q(DeckModel);

    if (!item)
        return;

    TreeItem *parentItem = item->parent();
    QModelIndex parent = (parentItem == rootItem) ?
                QModelIndex() :
                q->createIndex(parentItem->row(), 0, parentItem);
    QModelIndex child = q->index(item->row(), 0, parent);

    item->count = count;

    emit q->dataChanged(child, child);
}

TreeItem * DeckModelPrivate::findCard(const QString &group, const Card &card)
{
    QStringList groups;
    // FIXME: hardcoded
    groups << group << getCardType(card);

    TreeItem *item = rootItem;
    for (int i = 0; i < groups.size(); i++) {
        item = item->childGroups.value(groups[i]);
        if (!item)
            return 0;
    }
    return item->childCards.value(card);
}

void DeckModelPrivate::onCardAdded(const Card &card, const QString &group)
{
    Q_Q(DeckModel);

    QStringList groups;
    // FIXME: hardcoded
    groups << group << getCardType(card);

    TreeItem *item = rootItem;
    for (int i = 0; i < groups.size(); i++) {

        TreeItem* cachedItem = item->childGroups.value(groups[i]);
        if (cachedItem) {
            item = cachedItem;

            updateItemCount(item, item->count + 1);
        } else {
            QModelIndex parent = (item == rootItem) ? QModelIndex() : q->createIndex(item->row(), 0, item);
            q->beginInsertRows(parent, item->childCount(), item->childCount());

            item = new TreeItem(item, groups[i]);
            item->count = 1;

            q->endInsertRows();
        }
    }

    QModelIndex parent = q->createIndex(item->row(), 0, item);
    q->beginInsertRows(parent, item->childCount(), item->childCount());

    item = new TreeItem(item, card);

    q->endInsertRows();
}

void DeckModelPrivate::onCardRemoved(const Card &card, const QString &group)
{
    Q_Q(DeckModel);
    // TODO: discuss

    QStringList groups;
    // FIXME: hardcoded
    groups << group << getCardType(card);

    TreeItem *item = findCard(group, card);
    if (!item)
        return;

    while (item->parent() != rootItem && item->parent()->childCount() == 1) {
        item = item->parent();
    }

    TreeItem *parentItem = item->parent();
    QModelIndex parent = (parentItem == rootItem) ?
                QModelIndex() :
                q->createIndex(parentItem->row(), 0, parentItem);
    q->beginRemoveRows(parent, item->row(), item->row());

    Q_ASSERT(item != rootItem);
    delete item;

    q->endRemoveRows();

}

void DeckModelPrivate::onCountChanged(const Card &card, const QString &group, int count)
{
    // TODO: discuss
    TreeItem *item = findCard(group, card);
    if (!item)
        return;

    int oldCount = item->count;
    int delta = count - oldCount;
//    item->count = count;
    updateItemCount(item, count);

    while ( (item = item->parent()) != rootItem) {
//        item->count += delta;
        updateItemCount(item, item->count + delta);
    }

}

DeckModel::DeckModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new DeckModelPrivate(this))
{
}

DeckModel::DeckModel(Deck *deck, QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new DeckModelPrivate(this))
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
        if (index.column() == 0)
            return item->count;

        if (item->type == TreeItem::GroupItem) {
            if (index.column() == 2)
                return item->group;
            else
                return "";
        } else {
            return item->card.attribute(d->keys[index.column() - 1]);
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0)
            return Qt::AlignRight;
    } else if (role == Qt::BackgroundRole) {
        // FIXME: Hardcoded
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item->type == TreeItem::GroupItem) {
            if (item->parent() == d->rootItem)
                return QColor(196, 196, 196);
            else
                return QColor(228, 228, 228);
        }
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
        if (section == 0)
            return "Count";
        return d->keys[section - 1];
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

    d->keys = deck->cardBase()->game()->cardAttributes();
    if (d->deck) {
        disconnect(d->deck, SIGNAL(cardAdded(Card,QString)),
                   d, SLOT(onCardAdded(Card,QString)));
        disconnect(d->deck, SIGNAL(cardRemoved(Card,QString)),
                   d, SLOT(onCardRemoved(Card,QString)));
        disconnect(d->deck, SIGNAL(countChanged(Card,QString,int)),
                   d, SLOT(onCountChanged(Card,QString,int)));
    }

    connect(deck, SIGNAL(cardAdded(Card,QString)), d, SLOT(onCardAdded(Card,QString)));
    connect(deck, SIGNAL(cardRemoved(Card,QString)), d, SLOT(onCardRemoved(Card,QString)));
    connect(deck, SIGNAL(countChanged(Card,QString,int)), d, SLOT(onCountChanged(Card,QString,int)));
    d->deck = deck;
    d->buildTree();
}

QModelIndex DeckModel::topGroupIndex(const QModelIndex &idx)
{
    Q_D(DeckModel);

    TreeItem *item = static_cast<TreeItem*>(idx.internalPointer());
    while (item->parent() != d->rootItem) {
        item = item->parent();
    }
    return index(item->row(), 2, QModelIndex());
}

