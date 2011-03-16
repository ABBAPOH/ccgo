#ifndef XMLIOHANDLER_H
#define XMLIOHANDLER_H

#include "idatabaseiohandler.h"

class XmlIOHandler : public IDataBaseIOHandler
{
public:
    XmlIOHandler();

    virtual bool canHandle(const QString &);
    virtual bool canWrite() { return true; }

    virtual QString description();
    virtual QByteArray format();

    virtual bool read(const QString &, CardBase *base);
    virtual bool write(const QString &, const CardBase *base);
};

#endif // XMLIOHANDLER_H
