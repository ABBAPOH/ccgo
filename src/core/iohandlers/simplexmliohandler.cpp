#include "simplexmliohandler.h"

#include <QtCore/QFileInfo>
#include <QtXml/QXmlSimpleReader>

#include "simplexmlhandler.h"

SimpleXmlIOHandler::SimpleXmlIOHandler()
{
}

bool SimpleXmlIOHandler::canHandle(const QString &path)
{
    QFileInfo info(path);

    if (info.isFile() && info.exists() && info.suffix() == "xml") {
        return true;
    }
    return false;
}

bool SimpleXmlIOHandler::read(const QString &path, CardBase *base)
{

    SimpleXMLHandler handler(base);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);

    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(0, ("SAX Bookmarks"),
//                             QString("Cannot read file %1:\n%2.")
//                             .arg(path)
//                             .arg(file.errorString()));
        return false;
    }

    QXmlInputSource xmlInputSource(&file);

//    qDebug() << "finished reading" << reader.parse(xmlInputSource);
    return reader.parse(xmlInputSource);
}
