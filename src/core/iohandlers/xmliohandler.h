#ifndef XMLIOHANDLER_H
#define XMLIOHANDLER_H

#include "idatabaseiohandler.h"

class XmlIOHandler : public IDataBaseIOHandler
{
public:
    XmlIOHandler();

    virtual bool canHandle(const QString &);

    virtual bool read(const QString &, CardBase *base);
    virtual bool write(const QString &, const CardBase *base);
    virtual bool canWrite() { return true; }

};

#endif // XMLIOHANDLER_H
