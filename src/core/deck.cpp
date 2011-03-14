#include "deck.h"

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtCore/QFile>

#include <cardbase.h>
#include <game.h>
#include <QDebug>

quint64 qHash(const Card & card)
{
    return qHash(card.id());
}

bool operator <(const Card &first, const Card &second)
{
    return first.id() < second.id();
}

class DeckPrivate
{
public:
    CardBase *cardBase;

    QHash<QString, QHash<Card, int> > cards2;

};

Deck::Deck(CardBase *cardBase, QObject *parent) :
    QObject(parent),
    d_ptr(new DeckPrivate)
{
    Q_D(Deck);
    d->cardBase = cardBase;

}

Deck::~Deck()
{
    delete d_ptr;
}

CardBase * Deck::cardBase() const
{
    return d_func()->cardBase;
}

void Deck::addCard(const Card &card, const QString &group)
{
    Q_D(Deck);

    if (!d->cards2[group].contains(card)) {
        d->cards2[group][card] = 1;
        emit cardAdded(card, group);
    } else {
        int count = ++d->cards2[group][card];
        emit countChanged(card, group, count);
    }
}

void Deck::setCount(const Card &card, const QString &group, int count)
{
    Q_D(Deck);

    if (count >= 0) {
        d->cards2[group][card] = count;
        emit countChanged(card, group, count);
    }
}

void Deck::removeCard(const Card &card, const QString &group)
{
    Q_D(Deck);

    if (!d->cards2[group].contains(card))
        return;

    int &count = d->cards2[group][card];
    if (count <= 1) {
        d->cards2[group].remove(card);
        emit cardRemoved(card, group);
    } else {
        count--;
        emit countChanged(card, group, count);
    }
}

void Deck::removeAllCards(const Card &card, const QString &group)
{
    Q_D(Deck);

    if (!d->cards2[group].contains(card))
        return;

    d->cards2[group].remove(card);
    emit cardRemoved(card, group);
}

void Deck::clear()
{
    d_func()->cards2.clear();
}

QList<Card> Deck::cards() const
{
    QList<Card> result;
    foreach (const QString &group, groups()) {
        result.append(cards(group));
    }
    return result;
}

QList<Card> Deck::cards(const QString &group) const
{
    Q_D(const Deck);

    QList<Card> result;
    foreach (const Card &card, d->cards2[group].keys()) {
        int count = d->cards2[group][card];
        for (int i = 0; i < count; i++) {
            result.append(card);
        }
    }
    return result;
}

QList<Card> Deck::cardsSet() const
{
    QSet<Card> result;
    foreach (const QString &group, groups()) {
        QList<Card> tmp = cardsSet(group);
        for (int i = 0; i < tmp.size(); i++) {
            result.insert(tmp[i]);
        }
    }
    return result.toList();
}

int Deck::count(const Card &card) const
{
    int result = 0;
    foreach (const QString &group, groups()) {
        result += count(card, group);
    }
    return result;
}

int Deck::count(const Card &card, const QString &group) const
{
    Q_D(const Deck);

    return d->cards2[group].value(card, 0);
}

QList<Card> Deck::cardsSet(const QString &group) const
{
    Q_D(const Deck);

    return d->cards2[group].keys();
}

QStringList Deck::groups() const
{
    return d_func()->cardBase->game()->deckGroups();
}

void Deck::load(const QString &path)
{
    Q_D(Deck);

//    QSettings settings(file, QSettings::IniFormat);
//    foreach (const QString &group, groups()) {
//        int size = settings.beginReadArray(group);
//        for (int i = 0; i < size; i++) {
//            settings.setArrayIndex(i);
//            QString id = settings.value("1").toString();
//            Card card = d->cardBase->card(id);
//            addCard(card, group);
//        }
//        settings.endArray();
//    }

    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;

    while (!file.atEnd()) {
        bool ok = true;
        /*QByteArray*/QString string = file.readLine();
        string = string.trimmed();
        if (string.startsWith("//")) {
            qDebug() << "comment" << string;
            continue;
        }
        if (string.isEmpty())
            continue;
        if (string.startsWith("SB:")) {
            string = string.mid(3);
            qDebug() << string;
            string = string.trimmed();

            QString countString = string.left(string.indexOf(' '));
            qDebug() << countString;
            int count = countString.toInt(&ok);
            string = string.mid(countString.length());
            string = string.trimmed();
            qDebug() << ok << string << count;

            Card card = d->cardBase->card(string);
            if (card.isValid()) {
                addCard(card, "Sideboard");
                setCount(card, "Sideboard", count);
            }
        } else {
            qDebug() << string;
            string = string.trimmed();

            QString countString = string.left(string.indexOf(' '));
            qDebug() << countString;
            int count = countString.toInt(&ok);
            string = string.mid(countString.length());
            string = string.trimmed();
            qDebug() << ok << string << count;

            Card card = d->cardBase->card(string);
            if (card.isValid()) {
                addCard(card, "Main Deck");
                setCount(card, "Main Deck", count);
            }
        }
    }

}

void Deck::save(const QString &path)
{
//    Q_D(Deck);

    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;

    foreach (const QString &group, groups()) {
        QList<Card> cards = cardsSet(group);

        QString pattern;
        if (group == "Sideboard")
            pattern = "SB: %1 [%2] %3 \n";
        else
            pattern = "    %1 [%2] %3 \n";

        file.write(QString("  // %1 \n").arg(group).toAscii());
        for (int i = 0; i < cards.size(); i++) {
            int count = this->count(cards[i], group);
            file.write(pattern.
                       arg(QString::number(count)).
                       arg(cards[i].attribute("edition").toString()).
                       arg(cards[i].attribute("name").toString()).
                       toAscii());
        }
    }
}
