#include "spoileriohandler.h"

#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QDebug>

#include <cardbase.h>

SpoilerIOHandler::SpoilerIOHandler()
{
    mapToAttribute.insert("Card Name", "Name");
    mapToAttribute.insert("Card Color", "Color");
    mapToAttribute.insert("Mana Cost", "Cost");
    mapToAttribute.insert("Type & Class", "Type");
    mapToAttribute.insert("Pow", "Power");
    mapToAttribute.insert("Tou", "Toughness");
    mapToAttribute.insert("Card Text", "Text");
    mapToAttribute.insert("Flavor Text", "Flavor");
    mapToAttribute.insert("Artist", "Artist");
    mapToAttribute.insert("Rarity", "Flavor");
    mapToAttribute.insert("Card #", "No");
}

bool SpoilerIOHandler::canHandle(const QString &path)
{
    QFileInfo info(path);

    if (info.isFile() && info.exists() && info.suffix() == "txt") {
        return true;
    }
    return false;
}

QByteArray SpoilerIOHandler::format()
{
    return "txt";
}

QString SpoilerIOHandler::description()
{
    return "This handler allows reading from spoiler text files";
}

bool SpoilerIOHandler::read(const QString &path, CardBase *base)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    while (!file.atEnd()) {
        readLine(file.readLine(), base);
    }

    return true;
}

void SpoilerIOHandler::setEdition(const QString &edition, const QString &editionName)
{
    this->edition = edition;
    this->editionName = editionName;
}

void SpoilerIOHandler::readLine(const QString &line, CardBase *base)
{
    // line looks like "     key1 / key2    /  key3   :  value1/value2   / value3"

    if (edition.isEmpty())
        return;

    QString trimmedLine = line.trimmed();
    if (trimmedLine.isEmpty()) {
        if (card.isValid()) {
            base->addCard(card);
        }
        card = Card();
        card.setAttribute("Edition", edition);
        card.setAttribute("Edition Name", editionName);
    }
    int index = trimmedLine.indexOf(':');
    if (index == -1) // no ':' in string
        return;

    QString keyString = trimmedLine.left(index).trimmed();
    QString valueString = trimmedLine.mid(index + 1).trimmed();

    QStringList keys = keyString.split('/');

    QStringList values;
    if (valueString.isEmpty()) // if no values at all, fill list with empty ones
        for (int i = 0; i < keys.size(); i++)
            values.append("");
    else
        values = valueString.split('/');

//    if (keys.size() > values.size()) // incorrect number of arguments
//        return;

    for (int i = 0; i < keys.size(); i++) {
        QString key = keys[i].trimmed();
        QString value = i < values.size() ? values[i].trimmed() : "";
        qDebug()  << key << " : " << value;
        if (mapToAttribute.contains(key)) {
            key = mapToAttribute.value(key);
            card.setAttribute(key, value);
        } else
                qDebug() << "unknown key" << key;
    }
}
