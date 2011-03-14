#ifndef DECK_H
#define DECK_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include <card.h>

class CardBase;
class DeckPrivate;
class Deck : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Deck)
public:
    Deck(CardBase *cardBase, QObject *parent = 0);
    ~Deck();

    CardBase *cardBase() const;

    void addCard(const Card &card, const QString &group);
    void setCount(const Card &card, const QString &group, int count);

    void removeCard(const Card &card, const QString &group);
    void removeAllCards(const Card &card, const QString &group);
    void clear();

    QList<Card> cards() const;
    QList<Card> cards(const QString &group) const;

    QList<Card> cardsSet() const;
    QList<Card> cardsSet(const QString &group) const;

    int count(const Card &card) const;
    int count(const Card &card, const QString &group) const;

    QStringList groups() const;

    void load(const QString &file);
    void save(const QString &file);

signals:
    void cardAdded(const Card &card, const QString &group);
    void cardRemoved(const Card &card, const QString &group);
    void countChanged(const Card &card, const QString &group, int count);

public slots:

protected:
    DeckPrivate *d_ptr;
};

#endif // DECK_H
