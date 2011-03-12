#include "pictureloader.h"
#include "pictureloader_p.h"

#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QDebug>

#include <card.h>

// TODO: hardcoded

void PictureLoaderPrivate::download(const QUrl &from, const QString &to)
{
    QNetworkReply *device = manager->get(QNetworkRequest(from));
    device->setProperty("path", to);

    connect(device, SIGNAL(readyRead()), SLOT(onReadyRead()));

    connect(device, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
}

void PictureLoaderPrivate::save(QNetworkReply *reply)
{
    QString path = reply->property("path").toString();

    QFile file(path);
    file.open(QFile::WriteOnly);
    file.write(reply->readAll());
//    reply->deleteLater();

    emit finished(path);
}

void PictureLoaderPrivate::onError(QNetworkReply::NetworkError code)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qDebug() << "onError" << reply->errorString();
}

PictureLoader::PictureLoader(Game *game) :
    QObject(game),
    d_ptr(new PictureLoaderPrivate)
{
    Q_D(PictureLoader);

    Q_ASSERT(game);
    Q_ASSERT(game->isValid());

    d->game = game;
    d->manager = new QNetworkAccessManager(d);

    connect(d->manager, SIGNAL(finished(QNetworkReply*)), d, SLOT(save(QNetworkReply*)));
    connect(d, SIGNAL(finished(QString)), SLOT(onFinish(QString)));
}

PictureLoader::~PictureLoader()
{
    delete d_ptr;
}

void PictureLoader::getImage(const Card &card)
{
    Q_D(PictureLoader);

    QString id = card.id();
    QString filePath = d->game->pictureFolder() + card.toString(d->game->pictureLocalPattern());

    QFileInfo info(filePath);
    if (info.exists()) {
        if (info.isDir()) {
//            QFile::remove(info.absolutePath());
            QDir(filePath).rmpath(filePath);
        } else {
            emit imageLoadedForCard(id, filePath);
            return;
        }
    }

    QDir dir = info.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    // TODO: implement multiple loading from servers
    Q_ASSERT(!d->game->pictureServers().isEmpty());
    QUrl pictureUrl(d->game->pictureServers().at(0));
    pictureUrl.setPath(pictureUrl.path() + card.toString(d->game->pictureServerPattern()));

    d->pathToId.insert(filePath, id);
    QMetaObject::invokeMethod(d, "download", Q_ARG(QUrl, pictureUrl), Q_ARG(QString, filePath));
}

void PictureLoader::onFinish(const QString &path)
{
    Q_D(PictureLoader);

    emit imageLoadedForCard(d->pathToId[path], path);
    d->pathToId.remove(path);
}

void PictureLoaderPrivate::onReadyRead()
{
}
