#include "card.h"

#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtCore/QHash>

class CardData : public QSharedData {
public:
    mutable QString fullName;

    QVariantMap attributes;

    void setFullName() const;
};

void CardData::setFullName() const
{
    Q_ASSERT(attributes.keys().contains("name") &&
             attributes.keys().contains("edition"));
    fullName = QString("[%2] %1").
            arg(attributes.value("name").toString()).
            arg(attributes.value("edition").toString());
}

Card::Card() : data(new CardData)
{
}

Card::Card(const Card &other) : data(other.data)
{
}

Card &Card::operator=(const Card &other)
{
    if (this != &other)
        data.operator=(other.data);
    return *this;
}

Card::~Card()
{
}

QString Card::name() const
{
    return attribute("name").toString();
}

void Card::setName(const QString &name)
{
    setAttribute("name", name);
}

QString Card::id() const
{
    if (data->fullName.isEmpty())
        data->setFullName();
    return data->fullName;
}

QVariant Card::attribute(const QString &key) const
{
    if (key == "id")
        return id();
    return data->attributes.value(key.toLower());
}

void Card::setAttribute(const QString &key, const QVariant &value)
{
    data->attributes.insert(key.toLower(), value);
}

QStringList Card::attributeNames() const
{
    return data->attributes.keys();
}

bool Card::isValid() const
{
// TODO: implement
//    foreach (QString key, game->attributes()) {
//        if (!d->attributes.containsKey(key))
//            return false;
//    }
    return true;
}

bool Card::operator ==(const Card &other)
{
    return id() == other.id();
}

bool Card::operator !=(const Card &other)
{
    return id() != other.id();
}

QVariantMap Card::attributes() const
{
    return data->attributes;
}

QString Card::toString() const
{
    return QString("%1 %2 %3").arg(id()).arg(name()).arg(attribute("edition").toString());
}

QString Card::toString(const QString &pattern) const
{
    QString result(pattern);
    QRegExp regExp("\\$\\{.*\\}");
    regExp.setMinimal(true);
    int pos = 0;
    while ( (pos = regExp.indexIn(result, pos)) != -1) {
        QString cap = regExp.cap(0);
        QString attribute = cap.mid(2, cap.length() - 3);
        QString value = this->attribute(attribute).toString();
        result.replace(cap, value);
    }
    return result;
}

