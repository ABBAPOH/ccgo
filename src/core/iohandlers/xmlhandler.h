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
                      const QString &qName, const QXmlAttributes &cardAttributes);
    virtual bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    virtual bool characters(const QString &str);
    virtual bool fatalError(const QXmlParseException &exception);
    virtual QString errorString() const;

private:
    CardBase *cardBase;
    Card currentCard;

    QString currentText;
    QString errorStr;

    bool readingCards;
    bool readingEditions;
    QStringList cardAttributes;
    QStringList cardAttributesEncoded;
    QStringList cardTextAttributes;
    QStringList cardTextAttributesEncoded;
    int currentTextAttribute;
};

#endif // XMLHANDLER_H
