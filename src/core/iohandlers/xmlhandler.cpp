#include "xmlhandler.h"

#include <cardbase.h>

#include <QDebug>

XmlHandler::XmlHandler(CardBase *base) :
    cardBase(base),
    readingCards(false),
    currentLongData(-1)
{
    // HARDCODED:
    attributes << "Name" << "Edition" << "Edition Name" << "Color" << "Type"
               << "Rarity" << "Cost" << "Text" << "CMC" << "Artist" << "Legal"
               << "Flavor" << "No" << "Power" << "Toughness";

    for (int i = 0; i < attributes.size(); i++) {
        QString attribute = attributes[i];
        //bools.append(longData.contains(attribute));
        attributesEncoded.append(attribute.replace(' ', '_'));
    }

    longData << "Text" << "Flavor";
    for (int i = 0; i < longData.size(); i++) {
        QString attribute = longData[i];
        //bools.append(longData.contains(attribute));
        longDataEncoded.append(attribute.replace(' ', '_')/*.replace('/', '-').toLower()*/);
    }
}

bool XmlHandler::startElement(const QString &namespaceURI,
                              const QString &localName,
                              const QString &qName,
                              const QXmlAttributes &attributes)
{
    //qDebug() << qName;
    if (qName == "cards") {
        //qDebug() << "starting" << qName;
        readingCards = true;
        return true;
    }

    if (qName == "card") {
        //qDebug() << "starting" << qName;
        currentCard = Card();
        for (int i = 0; i< attributes.length(); i++) {
//            qDebug() << attributes.value(i) << attributes.qName(i);
        }
        for (int i = 0; i < attributesEncoded.length(); i++) {
//            qDebug() << attributes.value(attributesEncoded[i]);
            currentCard.setAttribute(this->attributes[i], attributes.value(attributesEncoded[i]));
        }
        return true;
    }

    if ( (currentLongData = longDataEncoded.indexOf(qName)) != -1) {
        //qDebug() << "starting" << qName;
        currentText = "";
        return true;
    }

    return true;
}

bool XmlHandler::endElement(const QString &namespaceURI,
                            const QString &localName,
                            const QString &qName)
{
    //qDebug() << "end" << qName;
    if (qName == "cards") {
        //qDebug() << "ending" << qName;
        readingCards = false;
        return true;
    }

    if ( (currentLongData != -1) && (currentLongData == longDataEncoded.indexOf(qName)) ) {
        //qDebug() << "ending" << qName;
        currentCard.setAttribute(longData[currentLongData], currentText);
        currentLongData = -1;
        return true;
    }

    if (qName == "card") {
        //qDebug() << "ending" << qName;
        cardBase->addCard(currentCard);
//        qDebug() << currentCard.attribute("name");
//        qDebug() << currentCard.attribute("flavor");
        return true;
    }

    return true;
}

bool XmlHandler::characters(const QString &str)
{
    //qDebug() << "characters" << str;
    currentText.append(str);
    return true;
}

bool XmlHandler::fatalError(const QXmlParseException &exception)
{
    //qDebug() << "fatalError" << exception.message() << exception.lineNumber() << exception.columnNumber();

    return false;
}

QString XmlHandler::errorString() const
{
    return errorStr;
}
