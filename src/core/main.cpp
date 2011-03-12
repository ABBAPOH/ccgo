#include <QtGui/QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    QMetaObject::invokeMethod(&w, "loadWindow", Qt::QueuedConnection);

    return app.exec();
}
