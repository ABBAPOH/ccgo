#ifndef DECKMODEL_P_H
#define DECKMODEL_P_H

#include <QtCore/QList>

#include <card.h>
#include <deck.h>

struct TreeItem
{
    TreeItem *m_parent;
    QList<TreeItem *> m_children;
    Card card;
    int count;
    QString group;
    bool hasCard;

    TreeItem(TreeItem *parent = 0)
    {
        m_parent = parent;
        if (parent)
            parent->m_children.append(this);
        count = 0;
        hasCard = false;
    }

    ~TreeItem()
    {
        foreach (TreeItem *item, m_children) {
            delete item;
        }
        m_children.clear(); // ??
        if (m_parent)
            m_parent->m_children.removeAll(this);
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

class DeckModelPrivate
{
public:
    DeckModelPrivate();
    ~DeckModelPrivate();

    void buildTree();
    TreeItem *createItem(TreeItem *groupItem, const Card &card);
    TreeItem *findItem(const QString &group);
    TreeItem *findItem(TreeItem *groupItem, const Card &card);

    TreeItem *rootItem;

    Deck *deck;

    QStringList keys;
};

#endif // DECKMODEL_P_H
