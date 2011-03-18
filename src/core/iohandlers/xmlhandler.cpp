#include "xmlhandler.h"

#include <cardbase.h>
#include <edition.h>
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
        if (!game->cardTextAttributes().contains(attribute) && attribute != "id") {
            cardAttributes.append(attribute);
        }
    }

    cardAttributesEncoded = Game::encode(cardAttributes);

    cardTextAttributes = game->cardTextAttributes();
    cardTextAttributesEncoded = Game::encode(cardTextAttributes);
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

    if (qName == "editions") {
        readingEditions = true;
        return true;
    }

    if (qName == "edition") {
        if (!readingEditions) {
            errorStr = "Met 'edition' tag not within 'editions' block.";
            return false;
        }
        QString key = attributes.value("key");
        Edition *edition = cardBase->edition(key);
        if (!edition)
            edition = new Edition();
        edition->setKey(key);
        edition->setName(attributes.value("name"));
        edition->setDescription(attributes.value("description"));
        edition->setDate(QDate::fromString(attributes.value("date"), "MM/yyyy"));
        cardBase->addEdition(edition);
//        qDebug() << edition->key() << edition->name() << edition->description() << edition->date();
        return true;
    }

    if (qName == "card") {
        currentCard = Card();
        for (int i = 0; i < cardAttributes.length(); i++) {
//            qDebug() << i << attributes.value(cardAttributesEncoded[i]);
            currentCard.setAttribute(cardAttributes[i], attributes.value(cardAttributesEncoded[i]));
        }
//        qDebug() << currentCard.toString();
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
    if (qName == "editions") {
        readingEditions = false;
        return true;
    }

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
            arg(exception.lineNumber()).
            arg(exception.columnNumber()).
            arg(exception.message());
    qWarning() << "fatalError" << errorStr;
    return false;
}

QString XmlHandler::errorString() const
{
    return errorStr;
}
