#include <QCoreApplication>

#include <QDebug>
#include <QDBusConnection>
#include <QDBusError>
#include <QObject>

#include <csignal>

#include "serviceprovider.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);

    QObject obj;
    auto service = new ServiceProvider(&obj);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, service, &ServiceProvider::shutdown);
    signal(SIGINT, app.exit);
    signal(SIGTERM, app.exit);

    QDBusConnection::sessionBus().registerObject("/", &obj);

    if (!QDBusConnection::sessionBus().registerService("org.nubecula.apocalypse")) {
        qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

    int retval = app.exec();

    delete service;

    return retval;
}
