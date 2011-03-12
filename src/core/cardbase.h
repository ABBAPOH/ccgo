#ifndef CARDBASE_H
#define CARDBASE_H

#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>

#include "card.h"
#include "iohandlers/idatabaseiohandler.h"

class Game;
class CardBasePrivate;
class CardBase : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CardBase)
public:
    explicit CardBase(Game *game);
    ~CardBase();

    QSqlDatabase dataBase();

    void addCard(const Card &card);
    Card card(const QString &id);

    QList<Card> cards() const;

    void addHandler(IDataBaseIOHandler *handler);
    void removeHandler(IDataBaseIOHandler *handler);

    bool importBase(const QString &path);
    bool exportBase(const QString &path, const QByteArray &format = QByteArray());
    void clearBase();

    QString errorString() const;

signals:

public slots:

protected:
    CardBasePrivate *d_ptr;
};

#endif // CARDBASE_H
