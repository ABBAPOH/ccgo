#ifndef SIMPLEXMLIOHANDLER_H
#define SIMPLEXMLIOHANDLER_H

#include "idatabaseiohandler.h"

class SimpleXmlIOHandler : public IDataBaseIOHandler
{
public:
    SimpleXmlIOHandler();

    virtual bool canHandle(const QString &);

    virtual bool read(const QString &, CardBase *base);
};

#endif // SIMPLEXMLIOHANDLER_H
