#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QtXml/QXmlDefaultHandler>

#include <card.h>

class CardBase;
class XmlHandler : public QXmlDefaultHandler
{
public:
    XmlHandler(CardBase *base);

    virtual bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    virtual bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    virtual bool characters(const QString &str);
    virtual bool fatalError(const QXmlParseException &exception);
    virtual QString errorString() const;

private:
    CardBase *cardBase;

    QString currentText;
    QString errorStr;
    bool readingCards;
    QStringList attributes;
    QStringList attributesEncoded;
    QStringList longData;
    QStringList longDataEncoded;
    int currentLongData;

    Card currentCard;

};

#endif // XMLHANDLER_H
