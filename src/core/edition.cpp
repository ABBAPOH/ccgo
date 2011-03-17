#include "edition.h"

#include "cardbase.h"

class EditionPrivate
{
public:
    QString key;
    QString name;
    QString description;
    QDate date;
    QIcon icon;
};

Edition::Edition(QObject *parent) :
    QObject(parent),
    d_ptr(new EditionPrivate)
{
}

Edition::~Edition()
{
    delete d_ptr;
}

CardBase * Edition::cardBase() const
{
    return qobject_cast<CardBase *>(parent());
}

QString Edition::key() const
{
    return d_func()->key;
}

void Edition::setKey(const QString &key)
{
    Q_D(Edition);

    if (d->key != key) {
        d->key = key;
        emit keyChanged(key);
    }
}

QString Edition::name() const
{
    return d_func()->name;
}

void Edition::setName(const QString &name)
{
    Q_D(Edition);

    if (d->name != name) {
        d->name = name;
        emit nameChanged(name);
    }
}

QString Edition::description() const
{
    return d_func()->description;
}

void Edition::setDescription(const QString &description)
{
    Q_D(Edition);

    if (d->description != description) {
        d->description = description;
        emit descriptionChanged(description);
    }
}

QDate Edition::date() const
{
    return d_func()->date;
}

void Edition::setDate(const QDate &date)
{
    Q_D(Edition);

    if (d->date != date) {
        d->date = date;
        emit dateChanged(date);
    }
}

QIcon Edition::icon() const
{
    return d_func()->icon;
}

void Edition::setIcon(const QIcon &icon)
{
    Q_D(Edition);

    d->icon = icon;
    emit iconChanged(icon);
}

