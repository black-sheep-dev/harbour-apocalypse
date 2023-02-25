#include <QCoreApplication>

#include <QDebug>
#include <QDBusConnection>
#include <QDBusError>
#include <QFile>
#include <QMutex>
#include <QObject>
#include <QStandardPaths>

#include <csignal>

#include "serviceprovider.h"

#ifdef QT_DEBUG

//some constants to parameterize.
const qint64 LOG_FILE_LIMIT = 3000000;
const QString LOG_PATH = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.nubecula/apocalypse";
const QString LOG_FILENAME = "/daemon.log";

void redirectDebugMessages(QtMsgType type, const QMessageLogContext & context, const QString & str)
{
    Q_UNUSED(context)

    //thread safety
    QMutex mutex;
    mutex.lock();
    QString txt;

    //prepend timestamp to every message
    QString datetime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    //prepend a log level label to every message
    switch (type) {
    case QtDebugMsg:
        txt = QString("[Debug*] ");
        break;
    case QtWarningMsg:
        txt = QString("[Warning] ");
    break;
    case QtInfoMsg:
        txt = QString("[Info] ");
    break;
    case QtCriticalMsg:
        txt = QString("[Critical] ");
    break;
    case QtFatalMsg:
        txt = QString("[Fatal] ");
    }

    QString filePath = LOG_PATH + LOG_FILENAME;
    QFile outFile(filePath);

    //if file reached the limit, rotate to filename.1
    if(outFile.size() > LOG_FILE_LIMIT){
        //roll the log file.
        QFile::remove(filePath + ".1");
        QFile::rename(filePath, filePath + ".1");
        QFile::resize(filePath, 0);
    }

    //write message
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << datetime << txt << str << endl;

    //close fd
    outFile.close();
    mutex.unlock();
}

#endif

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    qInstallMessageHandler(redirectDebugMessages);
#endif

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
