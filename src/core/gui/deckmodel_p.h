#ifndef DECKMODEL_P_H
#define DECKMODEL_P_H

#include <QtCore/QList>
#include <QtGui/QColor>

#include <game.h>
#include <card.h>
#include <cardbase.h>
#include <deck.h>

quint64 qHash(const Card & card);

struct TreeItem
{
    TreeItem *m_parent;
    QList<TreeItem *> m_children;

    int count;
    QString group;
    Card card;
    enum Type { RootItem = 0, GroupItem, CardItem };
    Type type;
    QMap<QString, TreeItem *> childGroups;
    QHash<Card, TreeItem *> childCards;

    TreeItem(TreeItem *parent = 0)
    {
        m_parent = parent;
        if (parent)
            parent->m_children.append(this);
        count = 0;
        type = RootItem;
    }

    TreeItem(TreeItem *parent, const QString &group)
    {
        m_parent = parent;
        if (parent) {
            parent->m_children.append(this);
            parent->childGroups.insert(group, this);
        }
        type = TreeItem::GroupItem;
        this->group = group;
        count = 0;
    }

    TreeItem(TreeItem *parent, const Card &card)
    {
        m_parent = parent;
        if (parent) {
            parent->m_children.append(this);
            parent->childCards.insert(card, this);
        }
        type = TreeItem::CardItem;
        this->card = card;
        count = 1;
    }

    ~TreeItem()
    {
        foreach (TreeItem *item, m_children) {
            delete item;
        }
        m_children.clear(); // ??
        if (m_parent) {
            m_parent->m_children.removeAll(this);
            if (type == GroupItem)
                m_parent->childGroups.remove(group);
            if (type == CardItem)
                m_parent->childCards.remove(card);
        }
    }

    int childCount()
    {
        return m_children.count();
    }

    TreeItem *parent()
    {
        return m_parent;
    }

    TreeItem *child(int row)
    {
        return m_children.at(row);
    }

    int row()
    {
        if (m_parent) {
            return m_parent->m_children.indexOf(this);
        }
        return 0;
    }
};

class DeckModel;
class DeckModelPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(DeckModel)
public:
    DeckModelPrivate(DeckModel *qq);
    ~DeckModelPrivate();

    void buildTree();

    void updateItemCount(TreeItem *item, int count);
    TreeItem *findCard(const QString &group, const Card &card);

public slots:
    void onCardAdded(const QString &group, const Card &card);
    void onCardRemoved(const QString &group, const Card &card);
    void onCountChanged(const Card &card, const QString &group, int count);

protected:
    DeckModel *q_ptr;

public:
    TreeItem *rootItem;
    Deck *deck;
    QStringList keys;
    // TODO: discuss
//    QHash<QPair<QString, Card>, TreeItem*> mapToItem;
//    QHash<QString, TreeItem*> mapFromGroup;
};

#endif // DECKMODEL_P_H
