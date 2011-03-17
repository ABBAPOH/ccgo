#ifndef CARDBASE_P_H
#define CARDBASE_P_H

#include "cardbase.h"

#include "card.h"
#include "game.h"
#include "edition.h"

class CardBasePrivate : public QObject
{
    Q_OBJECT
public:
    CardBasePrivate();

    Game *game;

    QSqlDatabase db;
    QMap<QByteArray, IDataBaseIOHandler *> handlers;

    QString addCardQuery;
    QString getCardQuery;
    QString getCardsQuery;
    QString errorString;

    QMap<QString, Edition *> editions;

    Card createCard(const QSqlRecord &record) const;
    void getSets();
    void addEdition(Edition *edition);

public slots:
    void onEditionKeyChanged(const QString &name);
};

#endif // CARDBASE_P_H
