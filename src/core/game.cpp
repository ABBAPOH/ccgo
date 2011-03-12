#include "game.h"

#include <QtCore/QFileInfo>
#include <QtCore/QSettings>
#include <QtCore/QDir>

#include <QDebug>

#include <config.h>

class GamePrivate
{
public:
    bool valid;
    QString name;
    QString gamePath;
    QStringList cardAttributes;
    QStringList deckGroups;
    QList<QUrl> pictureServers;
    QString pictureServerPattern;
    QString pictureFolder;
    QString pictureLocalPattern;

    QString errorString;

//    class GameBaseConfig
//    {
//    public:
//        QStringList attributes;

//        QStringList databaseNames;
//        QStringList types;
//        QStringList indexedColumns;
//        QStringList visibleAttributes; // program-specific
//    } config;

//    QStringList deckGroups;

};

Game::Game(const QString &name, QObject *parent) :
    QObject(parent),
    d_ptr(new GamePrivate)
{
    Q_D(Game);

    d->name = name;
    d->valid = false;
    d->errorString = tr("No error");

    setObjectName(QString("%1-Game").arg(name));

    Config *config = Config::instance();
    if (!config->isValid()) {
        d->errorString = tr("Config is not valid");
        return;
    }

    QString gamesPath = config->gamesPath();
    QFileInfo info(gamesPath + "/" + d->name + "/" + d->name);
    if (!info.exists() || !info.isFile()) {
        d->errorString = tr("Game config is not exist or is not a file");
        return;
    }
    d->gamePath = info.absolutePath();

    QSettings settings(info.absoluteFilePath(), QSettings::IniFormat);

    d->cardAttributes = settings.value("cardAttributes").toStringList();
    if (d->cardAttributes.size() == 0) {
        d->errorString = tr("cardAttributes list has zero size");
        return;
    }

    d->deckGroups = settings.value("deckGroups").toStringList();
    if (d->deckGroups.size() == 0) {
        d->deckGroups.append("Main Deck");
    }

    QStringList urls = settings.value("pictureServers").toStringList();
    for (int i = 0; i < urls.size(); i++) {
        QUrl url(urls[i]);
        if (url.isValid())
            d->pictureServers.append(url);
    }

    d->pictureFolder = "card images";
    info = QFileInfo(d->gamePath + "/" + d->pictureFolder);

    if (info.isFile())
        QFile::remove(info.absolutePath());

    if (!info.exists()) {
        QDir dir(info.absoluteDir());
        dir.mkdir(d->pictureFolder);
    }

    info.refresh(); // need to correctly calculate canonical file path if cached that dir is not exists
    d->pictureFolder = info.canonicalFilePath();
    qDebug() << "pictureFolder" << d->pictureFolder;

    d->pictureLocalPattern = settings.value("pictureLocalPattern").toString();
    d->pictureServerPattern = settings.value("pictureServerPattern").toString();

    d->valid = true;

//    config.attributes << "id" << "Name" << "Edition" << "Edition Name" << "Color" << "Type"
//                      << "Rarity" << "Cost" << "Text" << "CMC" << "Artist" << "Legal"
//                      << "Flavor" << "No" << "Power" << "Toughness";
//    config.databaseNames << "id" << "name" << "edition" << "edition_name" << "color" << "cardtype"
//                         << "rarity" << "cost" << "cardtext" << "cmc" << "artist" << "legal"
//                         << "flavor" << "no" << "power" << "toughness";
//    config.types << "varchar(20) primary key" << "varchar(20)" << "varchar(4)" << "varchar(20)"
//                 << "varchar(3)" << "varchar(20)" << "varchar(1)" << "varchar(20)" << "text"
//                 << "int" << "varchar(20)" << "varchar(20)" << "text" << "int"
//                 << "varchar(20)" << "varchar(20)";
//    config.indexedColumns << "name" << "edition" << "edition_name";

    //    d->deckGroups << "Main Deck" << "Sideboard";

}

Game::~Game()
{
    delete d_ptr;
}

bool Game::isValid() const
{
    return d_func()->valid;
}

QString Game::name() const
{
    return d_func()->name;
}

QString Game::gamePath() const
{
    return d_func()->gamePath;
}

QStringList Game::cardAttributes() const
{
    return d_func()->cardAttributes;
}

QStringList Game::deckGroups() const
{
    return d_func()->deckGroups;
}

QString Game::errorString() const
{
    return d_func()->errorString;
}

QList<QUrl> Game::pictureServers() const
{
    return d_func()->pictureServers;
}

QString Game::pictureServerPattern() const
{
    return d_func()->pictureServerPattern;
}

QString Game::pictureFolder() const
{
    return d_func()->pictureFolder;
}

QString Game::pictureLocalPattern() const
{
    return d_func()->pictureLocalPattern;
}

