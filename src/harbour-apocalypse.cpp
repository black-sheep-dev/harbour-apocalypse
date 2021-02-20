#include <QtQuick>

#include <sailfishapp.h>

#include "src/models/categoriesmodel.h"
#include "src/models/messagesortfiltermodel.h"
#include "src/service/serviceprovider.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qmlRegisterType<CategoriesModel>("org.nubecula.harbour.apocalypse", 1, 0, "CategoriesModel");
    qmlRegisterType<Location>("org.nubecula.harbour.apocalypse", 1, 0, "Location");
    qmlRegisterType<LocationModel>("org.nubecula.harbour.apocalypse", 1, 0, "LocationModel");
    qmlRegisterType<Message>("org.nubecula.harbour.apocalypse", 1, 0, "Message");
    qmlRegisterType<MessageModel>("org.nubecula.harbour.apocalypse", 1, 0, "MessageModel");
    qmlRegisterType<MessageSortFilterModel>("org.nubecula.harbour.apocalypse", 1, 0, "MessageSortFilterModel");
    qmlRegisterType<Service>("org.nubecula.harbour.apocalypse", 1, 0, "Service");
    qmlRegisterType<ServiceModel>("org.nubecula.harbour.apocalypse", 1, 0, "ServiceModel");

    qmlRegisterSingletonType<ServiceProvider>("org.nubecula.harbour.apocalypse",
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
