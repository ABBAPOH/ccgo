#include "simplexmlhandler.h"

#include <QMessageBox>
#include <QDebug>

SimpleXMLHandler::SimpleXMLHandler(CardBase *cardBase):
    readingCards(false)
{
    this->cardBase = cardBase;
}

bool SimpleXMLHandler::endDocument()
{
//    qDebug() << cards.at(111).id();
    return QXmlDefaultHandler::endDocument();
}

bool SimpleXMLHandler::startElement(const QString &namespaceURI, const QString &localName,
                                    const QString &qName, const QXmlAttributes &attributes)
{
    // maybe check for double opening lines?
//qDebug("lol");
    if (qName == "LINES") {
        bool ok1 = true, ok2 = true;
        row = 0;
        readingCards = true;
        properties.resize(attributes.value("ColCount").toInt(&ok1));
//        cards.resize(attributes.value("RowCount").toInt(&ok2) - 1);
        if (!ok1 || !ok2 ) {
            errorStr = "Can't convert string to double.";
            return false;
        }
    } else if (!readingCards) {
        return true;
    } else if (qName == "LINE") {
        column = 0;
        currentCard = Card();
    } else if (qName == "CELL") {
        currentText.clear();
    }

    return true;
}

bool SimpleXMLHandler::endElement(const QString &namespaceURI, const QString &localName,
                                  const QString &qName)
{
    if (!readingCards)
        return true;

    // maybe check for double closing lines?

    if (qName == "LINES") {
        readingCards = false;
    }

    if (qName == "CELL") {
        if (row == 0) {
            return true; // skipping empty head
        } else if (row == 1) {
            properties[column] = currentText;
        } else {
            currentCard.setAttribute(properties[column], currentText);
        }
        column++;
    }

    if (qName == "LINE") {
        if (row >= 2) {
            // dumping card
//            if (cards.size() == row - 2) {
//                errorStr = "error size";
//                return false;
//            }
//            cards[row - 2] = currentCard;
            cardBase->addCard(currentCard);
        }
        row++;
    }

    return true;
}

bool SimpleXMLHandler::characters(const QString &str)
{
    if (readingCards)
        currentText.append(str);
    return true;
}

bool SimpleXMLHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(0, QObject::tr("SAX Bookmarks"),
                              QObject::tr("Parse error at line %1, column %2:\n"
                                          "%3")
                              .arg(exception.lineNumber())
                              .arg(exception.columnNumber())
                              .arg(exception.message()));
     return false;
}

QString SimpleXMLHandler::errorString() const
{
    return errorStr;
}
