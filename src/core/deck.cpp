#include "deck.h"

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSettings>

#include <cardbase.h>

class DeckPrivate
{
public:
    CardBase *cardBase;

    QHash<QString, QList<Card> > cards;

    QStringList groups;
};

Deck::Deck(CardBase *cardBase, QObject *parent) :
    QObject(parent),
    d_ptr(new DeckPrivate)
{
    Q_D(Deck);
    d->cardBase = cardBase;

    d->groups << "Main Deck" << "Sideboard";
    foreach (QString s, d->groups) {
        d->cards.insert(s, QList<Card>());
    }
}

Deck::~Deck()
{
    delete d_ptr;
}

void Deck::addCard(const QString &group, const Card &card)
{
    Q_D(Deck);

    d->cards[group].append(card);
    emit cardAdded(group, card);
}

void Deck::removeCard(const QString &group, const Card &card)
{
    Q_D(Deck);

    d->cards[group].removeOne(card);
    emit cardRemoved(group, card);
}

QList<Card> Deck::cards(const QString &group) const
{
    Q_D(const Deck);

    return d->cards.value(group);
}

QList<Card> Deck::cards() const
{
    Q_D(const Deck);

    QList<Card> result;
    foreach (const QString &group, d->groups) {
        result.append(d->cards.value(group));
    }
    return result;
}

QStringList Deck::groups() const
{
    return d_func()->groups;
}

void Deck::load(const QString &file)
{
    Q_D(Deck);

    QSettings settings(file, QSettings::IniFormat);
    foreach (const QString &group, groups()) {
        int size = settings.beginReadArray(group);
        for (int i = 0; i < size; i++) {
            settings.setArrayIndex(i);
            QString id = settings.value("1").toString();
            Card card = d->cardBase->card(id);
            addCard(group, card);
        }
        settings.endArray();
    }
}

void Deck::save(const QString &file)
{
    Q_D(Deck);

    QSettings settings(file, QSettings::IniFormat);
    foreach (const QString &group, groups()) {
        settings.beginWriteArray(group);
        for (int i = 0; i < d->cards[group].size(); i++) {
            Card card = d->cards[group][i];
            settings.setArrayIndex(i);
            settings.setValue("1", card.id());
        }
        settings.endArray();
    }
}
