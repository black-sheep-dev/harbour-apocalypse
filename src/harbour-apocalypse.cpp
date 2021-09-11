#include <QtQuick>

#include <sailfishapp.h>

#include "src/models/categoriesmodel.h"
#include "src/models/messagesortfiltermodel.h"
#include "src/service/serviceprovider.h"

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    #define uri "org.nubecula.harbour.apocalypse"
#else
    const auto uri = "org.nubecula.harbour.apocalypse";
#endif


    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setApplicationName(APP_TARGET);
    QCoreApplication::setOrganizationName("nubecula.org");
    QCoreApplication::setOrganizationDomain("nubecula.org");

    qmlRegisterType<CategoriesModel>(uri, 1, 0, "CategoriesModel");
    qmlRegisterType<Location>(uri, 1, 0, "Location");
    qmlRegisterType<LocationModel>(uri, 1, 0, "LocationModel");
    qmlRegisterType<Message>(uri, 1, 0, "Message");
    qmlRegisterType<MessageModel>(uri, 1, 0, "MessageModel");
    qmlRegisterType<MessageSortFilterModel>(uri, 1, 0, "MessageSortFilterModel");
    qmlRegisterType<Service>(uri, 1, 0, "Service");
    qmlRegisterType<ServiceModel>(uri, 1, 0, "ServiceModel");

    qmlRegisterSingletonType<ServiceProvider>(uri,
                                              1,
                                              0,
                                              "ServiceProvider",
                                              [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        auto provider = new ServiceProvider();

        return provider;
    });

    return SailfishApp::main(argc, argv);
}
