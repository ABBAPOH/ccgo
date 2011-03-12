#ifndef IDATABASEIOHANDLER_H
#define IDATABASEIOHANDLER_H

#include <QtCore/QIODevice>

class CardBase;
class IDataBaseIOHandler
{
public:
    IDataBaseIOHandler() {}
    virtual ~IDataBaseIOHandler() {}

    virtual bool canHandle(const QString &) = 0;

    virtual bool read(const QString &, CardBase *base) = 0;
    virtual bool write(const QString &, const CardBase */*base*/) { return false; }
    virtual bool canWrite() { return false; }
    virtual QByteArray format() = 0;
    virtual QString description() { return ""; }
};

#endif // IDATABASEIOHANDLER_H
