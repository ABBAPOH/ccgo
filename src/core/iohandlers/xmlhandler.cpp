#include "xmlhandler.h"

#include <cardbase.h>
#include <game.h>

#include <QDebug>

XmlHandler::XmlHandler(CardBase *base) :
    cardBase(base),
    readingCards(false),
    currentTextAttribute(-1)
{
    Game *game = base->game();
    for (int i = 0; i < game->cardAttributes().size(); i++) {
        QString attribute = game->cardAttributes()[i];
        if (!game->cardTextAttributes().contains(attribute)) {
            cardAttributes.append(attribute);
            cardAttributesEncoded.append(game->cardAttributesEncoded());
        }
    }

    cardTextAttributes = game->cardTextAttributes();
    cardTextAttributesEncoded = game->cardTextAttributesEncoded();
}

bool XmlHandler::startElement(const QString &namespaceURI,
                              const QString &localName,
                              const QString &qName,
                              const QXmlAttributes &attributes)
{
    if (qName == "cards") {
        readingCards = true;
        return true;
    }

    if (qName == "card") {
        currentCard = Card();
        for (int i = 0; i < cardAttributesEncoded.length(); i++) {
            currentCard.setAttribute(cardAttributes[i], attributes.value(cardAttributesEncoded[i]));
        }
        return true;
    }

    if ( (currentTextAttribute = cardTextAttributesEncoded.indexOf(qName)) != -1) {
        currentText = "";
        return true;
    }

    return true;
}

bool XmlHandler::endElement(const QString &namespaceURI,
                            const QString &localName,
                            const QString &qName)
{
    if (qName == "cards") {
        readingCards = false;
        return true;
    }

    if ( (currentTextAttribute != -1) &&
            (currentTextAttribute == cardTextAttributesEncoded.indexOf(qName)) ) {
        currentCard.setAttribute(cardTextAttributes[currentTextAttribute], currentText);
        currentTextAttribute = -1;
        return true;
    }

    if (qName == "card") {
        cardBase->addCard(currentCard);
        return true;
    }

    return true;
}

bool XmlHandler::characters(const QString &str)
{
    currentText.append(str);
    return true;
}

bool XmlHandler::fatalError(const QXmlParseException &exception)
{
    errorStr = QString("Fatal error on line %1, column %2: %3").
            arg(exception.message()).
            arg(exception.lineNumber()).
            arg(exception.columnNumber());
    return false;
}

QString XmlHandler::errorString() const
{
    return errorStr;
}
