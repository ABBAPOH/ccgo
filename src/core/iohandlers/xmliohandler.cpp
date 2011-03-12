#include "xmliohandler.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QXmlStreamWriter>

#include <card.h>
#include <cardbase.h>
#include "xmlhandler.h"

#include <QDebug>

XmlIOHandler::XmlIOHandler()
{
}

bool XmlIOHandler::canHandle(const QString &path)
{
    QFileInfo info(path);

    // TODO: implement correctly
    if (/*info.isFile() && info.exists() && */info.suffix() == "xml") {
        return true;
    }
    return false;
}

bool XmlIOHandler::read(const QString &path, CardBase *base)
{
    XmlHandler handler(base);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);

    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(0, ("SAX Bookmarks"),
//                             QString("Cannot read file %1:\n%2.")
//                             .arg(path)
//                             .arg(file.errorString()));
        return false;
    }

    QXmlInputSource xmlInputSource(&file);

    return reader.parse(xmlInputSource);
}

bool XmlIOHandler::write(const QString &path, const CardBase *base)
{
    QStringList longData;
    longData << "Text" << "Flavor";

    QFile file(path);
    file.open(QFile::WriteOnly);

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument("1.0");

    writer.writeStartElement("cards");
    {
        QList<Card> cards = base->cards();
//        QStringList attributes = cards[0].attributeNames();
        QStringList attributes;
        // HARDCODED:
        attributes << "Name" << "Edition" << "Edition Name" << "Color" << "Type"
                       << "Rarity" << "Cost" << "Text" << "CMC" << "Artist" << "Legal"
                       << "Flavor" << "No" << "Power" << "Toughness";
        QStringList attributesEncoded;
        QList<bool> bools;

        for (int i = 0; i < attributes.size(); i++) {
            QString attribute = attributes[i];
            bools.append(longData.contains(attribute));
            attributesEncoded.append(attribute.replace(' ', '_')/*.replace('/', '-')*/);
        }

        for (int j = 0; j < cards.size(); j++) {
            writer.writeStartElement("card");

            const Card &card = cards[j];

            for (int i = 0; i < attributes.size(); i++) {
                if (!bools[i]) {
                    writer.writeAttribute(attributesEncoded[i],
                                          card.attribute(attributes[i]).toString());
                }
            }

            for (int i = 0; i < attributes.size(); i++) {
                if (bools[i]) {
                    writer.writeTextElement(attributesEncoded[i],
                                            card.attribute(attributes[i]).toString());
                }
            }

            writer.writeEndElement();
        }

    }
    writer.writeEndElement();

    writer.writeEndDocument();

    return true;
}
