#ifndef PICTURELOADER_H
#define PICTURELOADER_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

class Card;
class QIODevice;
class QFile;
class Game;
class PictureLoaderPrivate;
class PictureLoader : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PictureLoader)
public:
    explicit PictureLoader(Game *game);
    ~PictureLoader();

    void getImage(const Card &card);

signals:
    void imageLoadedForCard(const QString &id, const QString &path);

private slots:
    void onFinish(const QString &path);

protected:
    PictureLoaderPrivate *d_ptr;
};

#endif // PICTURELOADER_H
