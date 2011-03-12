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

    void addCard(const QString &group, const Card &card);
    void removeCard(const QString &group, const Card &card);

    QList<Card> cards(const QString &group) const;
    QList<Card> cards() const;

    QStringList groups() const;

    void load(const QString &file);
    void save(const QString &file);

signals:
    void cardAdded(const QString &group, const Card &card);
    void cardRemoved(const QString &group, const Card &card);

public slots:

protected:
    DeckPrivate *d_ptr;
};

#endif // DECK_H
