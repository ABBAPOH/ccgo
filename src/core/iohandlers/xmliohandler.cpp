#include "xmliohandler.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QXmlStreamWriter>

#include <card.h>
#include <cardbase.h>
#include <game.h>
#include <edition.h>
#include "xmlhandler.h"

#include <QDebug>

XmlIOHandler::XmlIOHandler()
{
}

bool XmlIOHandler::canHandle(const QString &path)
{
    QFileInfo info(path);

    if (info.isFile() && info.exists() && info.suffix() == "xml") {
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

    writer.writeStartElement("cardbase");

    writer.writeStartElement("editions");
    {
        QList<Edition *> editions = base->editions();
        for (int i = 0; i < editions.size(); i++) {
            Edition * edition = editions[i];
            writer.writeStartElement("edition");

            writer.writeAttribute("key", edition->key());
            writer.writeAttribute("name", edition->name());
            writer.writeAttribute("description", edition->description());
            writer.writeAttribute("date", edition->date().toString("MM/yyyy"));

            writer.writeEndElement();
        }
    }
    writer.writeEndElement();

    writer.writeStartElement("cards");
    {
        QList<Card> cards = base->cards();
//        QStringList attributes = cards[0].attributeNames();
        QStringList attributes;
        // HARDCODED:
        attributes << "id" << "Name" << "Edition" << "Edition Name" << "Color" << "Type"
                       << "Rarity" << "Cost" << "Text" << "CMC" << "Artist" << "Legal"
                       << "Flavor" << "No" << "Power" << "Toughness";
        attributes = base->game()->cardAttributes();
        attributes.takeFirst();
        QStringList attributesEncoded;
        QList<bool> bools;

        for (int i = 0; i < attributes.size(); i++) {
            QString attribute = attributes[i];
            bools.append(longData.contains(attribute));
            attributesEncoded.append(base->game()->cardAttributesEncoded()[i+1]);
//            attributesEncoded.append(attribute.replace(' ', '_')/*.replace('/', '-')*/);
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

    writer.writeEndElement();

    writer.writeEndDocument();

    return true;
}

QByteArray XmlIOHandler::format()
{
    return "xml";
}

QString XmlIOHandler::description()
{
    return "This is main handler for importing/exporting from/to xml files.";
}
