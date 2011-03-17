#include "cardbase.h"
#include "cardbase_p.h"

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtGui/QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#include <QtCore/QDebug>

CardBasePrivate::CardBasePrivate()
{
}

void CardBasePrivate::onEditionKeyChanged(const QString &key)
{
    Edition *edition = qobject_cast<Edition *>(sender());
    if (!edition)
        return;
    QString oldKey = editions.key(edition);
    if (!oldKey.isEmpty())
        editions.remove(oldKey);
    if (!key.isEmpty())
        editions.insert(key, edition);
}

Card CardBasePrivate::createCard(const QSqlRecord &record) const
{
    Card result;

    for (int i = 1; i < game->cardAttributes().size(); i++) {
        result.setAttribute(game->cardAttributes()[i], record.value(i).toString());
    }

    return result;
}

void CardBasePrivate::getSets()
{
    QSqlQuery query(db);
    query.exec("SELECT edition, edition_name FROM cardbase GROUP BY edition, edition_name");
    while(query.next()) {
        QString key = query.value(0).toString();
        if (!editions.contains(key)) {
            Edition *edition = new Edition;
            edition->setKey(key);
            edition->setName(query.value(1).toString());
            addEdition(edition);
        }
    }
}

void CardBasePrivate::addEdition(Edition *edition)
{
    if (!edition)
        return;

    if (!edition->key().isEmpty()) {
        if (editions.contains(edition->key()))
            return;
        editions.insert(edition->key(), edition);
    }

    edition->setParent(this);
    connect(edition, SIGNAL(keyChanged(QString)), this, SLOT(onEditionKeyChanged(QString)));
}

CardBase::CardBase(Game *game) :
    QObject(game),
    d_ptr(new CardBasePrivate)
{
    Q_D(CardBase);

    Q_ASSERT(game);
    Q_ASSERT(game->isValid());

    d->game = game;

    d->db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(":memory:");
    d->db.setDatabaseName(qApp->applicationDirPath() + "/base.sql");
    if (!d->db.open()) {
        qWarning() << "Can't open database";
        return;
    }

    QStringList attributes = d->game->cardAttributes();
    QStringList databaseNames = d->game->cardAttributesEncoded();
    QStringList types = d->game->dataBaseTypes();

    // TODO: move following to appropriate place
    QSqlQuery query(d->db);
    QString queryString = "";
    for (int i = 0; i < attributes.size(); i++) {
        queryString.append(QString("%1 %2").arg(databaseNames[i]).arg(types[i]));
        if (i != attributes.size() - 1) {
            queryString.append(", ");
        }
    }

    QString names = databaseNames.join(", ");;
    QString placeholders = ":" + databaseNames.join(", :");

    d->addCardQuery = QString("INSERT INTO cardbase (%1) VALUES (%2)").arg(names).arg(placeholders);
    d->getCardQuery = QString("SELECT %1 FROM cardbase WHERE id=:id").arg(names);
    d->getCardsQuery = QString("SELECT %1 FROM cardbase").arg(names);

    query.exec(QString("create table cardbase (%1)").arg(queryString));

//    for (int i = 0; i < config.indexedColumns.size(); i++) {
    for (int i = 0; i < databaseNames.size(); i++) {
        QString queryString = QString("CREATE INDEX IF NOT EXIST %1 ON cardbase (%2)").
                arg(databaseNames[i] + "_index").
                arg(databaseNames[i]);
//                        arg(config.indexedColumns[i] + "_index").
//                        arg(config.indexedColumns[i]);
        query.exec(queryString);
    }

    d->getSets();
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

    QStringList databaseNames = d->game->cardAttributesEncoded();
    QStringList cardAttributes = d->game->cardAttributes();
    query.prepare(d->addCardQuery);
//    qDebug() << d->addCardQuery;
//    qDebug() << d->addCardQuery << databaseNames << cardAttributes;
//    qDebug() << card.toString();
//    qDebug() << databaseNames;
//    qDebug() << cardAttributes;
    for (int i = 0; i < databaseNames.size(); i++) {
        query.bindValue(":" + databaseNames[i], card.attribute(cardAttributes[i]));
    }

    query.exec();
//    qDebug() << "add card" << query.exec() << query.lastError();
}

void CardBase::addHandler(IDataBaseIOHandler *handler)
{
    Q_D(CardBase);

    QByteArray format = handler->format();
    if (!d->handlers.contains(format))
        d->handlers.insert(format, handler);
}

void CardBase::removeHandler(IDataBaseIOHandler *handler)
{
    Q_D(CardBase);

    d->handlers.remove(handler->format());
}

Card CardBase::card(const QString &id)
{
    Q_D(CardBase);

    // TODO: reuse query with different execs
    QSqlQuery query(d->db);

    query.prepare(d->getCardQuery);
    query.bindValue(":id", id);
    query.exec();

    Card result;

    if (query.next()) {
        result = d->createCard(query.record());
    }

    return result;
}

QList<Card> CardBase::cards() const
{
    Q_D(const CardBase);

    QList<Card> result;

    QSqlQuery query(d->db);

    query.prepare(d->getCardsQuery);
    query.exec();

    while (query.next()) {
        result.append(d->createCard(query.record()));
    }

    return result;
}

bool CardBase::importBase(const QString &path)
{
    Q_D(CardBase);

    IDataBaseIOHandler *handler = 0;
    QList<IDataBaseIOHandler *> handlers = d->handlers.values();
    for (int i = 0; i < handlers.size(); i++) {
        IDataBaseIOHandler *h = handlers[i];
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

    d->getSets(); // we update information about _new_ sets

    return result;
}

bool CardBase::exportBase(const QString &path, const QByteArray &f)
{
    Q_D(CardBase);

    QByteArray format(f);
    if (format.isEmpty()) {
        format = QFileInfo(path).suffix().toAscii();
    }

    IDataBaseIOHandler *handler = d->handlers.value(format);
    if (!handler)
        return false;

    return handler->write(path, this);
}

void CardBase::clearBase()
{
    Q_D(CardBase);

    QSqlQuery query(d->db);
    query.exec("DELETE FROM cardbase");
}

QString CardBase::errorString() const
{
    return d_func()->errorString;
}

Game * CardBase::game() const
{
    return d_func()->game;
}

void CardBase::addEdition(Edition *edition)
{
    Q_D(CardBase);
    d->addEdition(edition);
}

Edition * CardBase::edition(const QString &key) const
{
    return d_func()->editions.value(key);
}

QList<Edition *> CardBase::editions() const
{
    return d_func()->editions.values();
}


