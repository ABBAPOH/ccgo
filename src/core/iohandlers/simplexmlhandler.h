#ifndef SIMPLEXMLHANDLER_H
#define SIMPLEXMLHANDLER_H

#include <QXmlDefaultHandler>
#include <QVector>

#include "card.h"
#include "cardbase.h"

class SimpleXMLHandler : public QXmlDefaultHandler
{
public:
    SimpleXMLHandler(CardBase *cardBase);

    virtual bool endDocument();
    virtual bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    virtual bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    virtual bool characters(const QString &str);
    virtual bool fatalError(const QXmlParseException &exception);
    virtual QString errorString() const;

private:
    QString currentText;
    QString errorStr;
    bool isSkipping;

    bool readingCards;
    int row, column;

    Card currentCard;
    QVector<Card> cards;
    QVector<QString> cardData;
    QVector<QString> properties;
    CardBase *cardBase;
};

#endif // SIMPLEXMLHANDLER_H
