#include "config.h"

#include <QtGui/QApplication>

Config::Config(QObject *parent) :
    QObject(parent)
{
    m_gamesPath = qApp->applicationDirPath() + "/games";
}

Config::~Config()
{
}

QString Config::gamesPath() const
{
    return m_gamesPath;
}

QSize Config::cardPictureSize() const
{
    return QSize(260, 360);
}

Config * Config::instance()
{
    static Config *config = new Config(qApp);
    return config;
}

bool Config::isValid() const
{
    // TODO: implement
    return true;
}
