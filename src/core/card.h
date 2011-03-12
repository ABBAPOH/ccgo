#ifndef CARD_H
#define CARD_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QVariantMap>
#include <QtCore/QStringList>

class CardData;

class Card
{
public:
    Card();
    Card(const Card &other);
    Card &operator=(const Card &);
    ~Card();

    QString name() const;
    void setName(const QString &name);

    QString id() const;

    QVariantMap attributes() const;
    QVariant attribute(const QString &key) const;
    void setAttribute(const QString &key, const QVariant &value);

    QStringList attributeNames() const;

    QString toString() const;
    QString toString(const QString &pattern) const;

    //    Row
    //    Name
    //    Edition
    //    Edition Name
    //    Color
    //    Type
    //    Rarity
    //    Cost
    //    P/T
    //    Text
    //    CMC
    //    Artist
    //    Legal
    //    Flavor
    //    No
    //    Power
    //    Toughness

    bool isValid() const;

    bool operator ==(const Card &other);
    bool operator !=(const Card &other);

private:
    QSharedDataPointer<CardData> data;
};

#endif // CARD_H
