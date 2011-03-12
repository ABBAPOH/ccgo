#ifndef GAME_H
#define GAME_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QUrl>

class Card;

class GamePrivate;
class Game : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Game)
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString gamePath READ gamePath)
    Q_PROPERTY(QStringList cardAttributes READ cardAttributes)
    Q_PROPERTY(QStringList deckGroups READ deckGroups)
    Q_PROPERTY(QList<QUrl> pictureServers READ pictureServers)
    Q_PROPERTY(QString pictureServerPattern READ pictureServerPattern)
    Q_PROPERTY(QString pictureFolder READ pictureFolder)
    Q_PROPERTY(QString pictureLocalPattern READ pictureLocalPattern)
public:
    explicit Game(const QString &name, QObject *parent = 0);
    ~Game();

    bool isValid() const;
    QString cardId(const Card &card);

    QString name() const;
    QString gamePath() const;
    QStringList cardAttributes() const;
    QStringList cardAttributesEncoded() const;
    QStringList dataBaseTypes() const;
    QStringList cardTextAttributes() const;
    QStringList cardTextAttributesEncoded() const;
    QStringList deckGroups() const;

    QList<QUrl> pictureServers() const;
    QString pictureServerPattern() const;

    QString pictureFolder() const;
    QString pictureLocalPattern() const;

    QString errorString() const;

signals:

public slots:

protected:
    GamePrivate *d_ptr;
};

#endif // GAME_H
