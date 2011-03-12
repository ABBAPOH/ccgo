#include "cardbase.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlError>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtGui/QApplication>

#include <QtCore/QDebug>

#include "card.h"
#include "game.h"

// TODO: batch exec

class CardBasePrivate
{
public:
    CardBasePrivate();

    Game *game;

    QSqlDatabase db;
    QList<IDataBaseIOHandler *> handlers;

    class GameBaseConfig
    {
    public:
        QStringList attributes;

        QStringList databaseNames;
        QStringList types;
        QStringList indexedColumns;
    } config;
    QString addCardQuery;
    QString getCardQuery;

    QList<Card> cards;
};

CardBasePrivate::CardBasePrivate()
{
    config.attributes << "id" << "Name" << "Edition" << "Edition Name" << "Color" << "Type"
                      << "Rarity" << "Cost" << "Text" << "CMC" << "Artist" << "Legal"
                      << "Flavor" << "No" << "Power" << "Toughness";
    config.databaseNames << "id" << "name" << "edition" << "edition_name" << "color" << "cardtype"
                         << "rarity" << "cost" << "cardtext" << "cmc" << "artist" << "legal"
                         << "flavor" << "no" << "power" << "toughness";
    config.types << "varchar(20) primary key" << "varchar(20)" << "varchar(4)" << "varchar(20)"
                 << "varchar(3)" << "varchar(20)" << "varchar(1)" << "varchar(20)" << "text"
                 << "int" << "varchar(20)" << "varchar(20)" << "text" << "int"
                 << "varchar(20)" << "varchar(20)";

    config.indexedColumns << "name" << "edition" << "edition_name";

    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(":memory:");
    db.setDatabaseName(qApp->applicationDirPath() + "/base3.sql");
    if (!db.open())
        qWarning() << "Can't open database";

    // TODO: move following to appropriate place
    QSqlQuery query(db);
    QString queryString = "";
    for (int i = 0; i < config.attributes.size(); i++) {
        queryString.append(QString("%1 %2").arg(config.databaseNames[i]).arg(config.types[i]));
        if (i != config.attributes.size() - 1) {
            queryString.append(", ");
        }
    }

    QString names = config.databaseNames.join(", ");;
    QString placeholders = ":" + config.databaseNames.join(", :");

    addCardQuery = QString("INSERT INTO cardbase (%1) VALUES (%2)").arg(names).arg(placeholders);

    getCardQuery = QString("SELECT %1 FROM cardbase WHERE id=:id").arg(names);

    query.exec(QString("create table cardbase (%1)").arg(queryString));

    for (int i = 0; i < config.indexedColumns.size(); i++) {
        QString queryString = QString("CREATE INDEX %1 ON cardbase (%2)").
                        arg(config.indexedColumns[i] + "_index").
                        arg(config.indexedColumns[i]);
        query.exec(queryString);
    }
}

CardBase::CardBase(Game *game) :
    QObject(game),
    d_ptr(new CardBasePrivate)
{
    Q_D(CardBase);

    Q_ASSERT(game);
    Q_ASSERT(game->isValid());

    d->game = game;
}

CardBase::~CardBase()
{
    Q_D(CardBase);

    d->db.close();

    qDeleteAll(d->handlers);

    delete d_ptr;
}

QSqlDatabase CardBase::dataBase()
{
    return d_func()->db;
}

void CardBase::addCard(const Card &card)
{
    Q_D(CardBase);

    // TODO: reuse query with different execs
    QSqlQuery query(d->db);

    query.prepare(d->addCardQuery);
    for (int i = 0; i< d->config.databaseNames.size(); i++) {
        query.bindValue(":" + d->config.databaseNames[i], card.attribute(d->config.attributes[i]));
    }

    query.exec();

    d->cards.append(card);
}

void CardBase::addHandler(IDataBaseIOHandler *handler)
{
    Q_D(CardBase);

    if (!d->handlers.contains(handler))
        d->handlers.append(handler);
}

void CardBase::removeHandler(IDataBaseIOHandler *handler)
{
    Q_D(CardBase);

    d->handlers.removeAll(handler);
}

Card CardBase::card(const QString &id)
{
    Q_D(CardBase);

    // TODO: reuse query with different execs
    QSqlQuery query(d->db);

    query.prepare(d->getCardQuery);
    query.bindValue(":id", id);
    query.exec();


    Card c;

    if (query.next()) {
        for (int i = 1; i< d->config.attributes.size(); i++) {
            c.setAttribute(d->config.attributes[i], query.value(i).toString());
        }
    }

    return c;
}

QList<Card> CardBase::cards() const
{
    return d_func()->cards;
}

bool CardBase::importBase(const QString &path)
{
    Q_D(CardBase);

    IDataBaseIOHandler *handler = 0;
    for (int i = 0; i < d->handlers.size(); i++) {
        IDataBaseIOHandler *h = d->handlers[i];
        if (h->canHandle(path)) {
            handler = h;
            break;
        }
    }

    if (!handler)
        return false;

    d->db.transaction();
    bool result;
    if (result = handler->read(path, this))
        d->db.commit();
    else
        d->db.rollback();

    return result;
}

bool CardBase::exportBase(const QString &path)
{
    Q_D(CardBase);

    IDataBaseIOHandler *handler = 0;
    for (int i = 0; i < d->handlers.size(); i++) {
        IDataBaseIOHandler *h = d->handlers[i];
        if (h->canHandle(path)) {
            handler = h;
            break;
        }
    }

    if (!handler)
        return false;

    return handler->write(path, this);
}
