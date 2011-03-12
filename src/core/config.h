#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore/QObject>
#include <QtCore/QString>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);
    ~Config();

    QString gamesPath() const;

    bool isValid() const;

    static Config *instance();

signals:

public slots:

private:
    QString m_gamesPath;
};

#endif // CONFIG_H
