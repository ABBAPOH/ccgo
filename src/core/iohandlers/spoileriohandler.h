#ifndef SPOILERIOHANDLER_H
#define SPOILERIOHANDLER_H

#include "idatabaseiohandler.h"

#include <QtCore/QMap>

#include <card.h>

class SpoilerIOHandler : public IDataBaseIOHandler
{
public:
    SpoilerIOHandler();

    virtual bool canHandle(const QString &);

    virtual QByteArray format();
    virtual QString description();

    virtual bool read(const QString &, CardBase *base);

    void setEdition(const QString &edition, const QString &editionName);

private:
    void readLine(const QString &line, CardBase *base);

    QString edition;
    QString editionName;
    QMap<QString, QString> mapToAttribute;
    Card card;
};

#endif // SPOILERIOHANDLER_H
