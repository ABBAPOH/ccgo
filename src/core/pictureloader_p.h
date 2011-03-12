#ifndef PICTURELOADER_P_H
#define PICTURELOADER_P_H

#include "pictureloader.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <game.h>

class PictureLoaderPrivate : public QObject
{
    Q_OBJECT
public:
    Game *game;
    QMap<QString, QString> pathToId;

    QNetworkAccessManager *manager; // within thread

public slots:
    void download(const QUrl &from, const QString &to);
    void save(QNetworkReply *reply);
    void onError(QNetworkReply::NetworkError code);
    void onReadyRead();

signals:
    void finished(const QString &);
};

#endif // PICTURELOADER_P_H
