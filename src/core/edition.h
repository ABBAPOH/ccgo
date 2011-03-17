#ifndef EDITION_H
#define EDITION_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtGui/QIcon>

class CardBase;
class EditionPrivate;
class Edition : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Edition)
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged)

public:
    explicit Edition(QObject *parent = 0);
    ~Edition();

    CardBase *cardBase() const;

    QString key() const; // short edition key aka MBS
    void setKey(const QString &key);

    QString name() const; // long name aka Mirrodin Besiged
    void setName(const QString &name);

    QString description() const; // edition description
    void setDescription(const QString &description);

    QDate date() const; // date when set was released
    void setDate(const QDate &date);

    QIcon icon() const; // edition icon
    void setIcon(const QIcon &icon);

signals:
    void keyChanged(const QString &key);
    void nameChanged(const QString &key);
    void descriptionChanged(const QString &key);
    void dateChanged(const QDate &key);
    void iconChanged(const QIcon &key);

public slots:

protected:
    EditionPrivate *d_ptr;
};

#endif // EDITION_H
