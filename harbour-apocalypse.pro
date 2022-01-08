# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# VERSION
VERSION = 0.3.5
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# The name of your application
TARGET = harbour-apocalypse
DEFINES += APP_TARGET=\\\"$$TARGET\\\"

CONFIG += sailfishapp

QT += positioning location

PKGCONFIG += \
    nemonotifications-qt5 \
    keepalive

LIBS += -lz

include(mapbox_api_key.pri)

SOURCES += src/harbour-apocalypse.cpp \
    src/entities/location.cpp \
    src/entities/message.cpp \
    src/entities/service.cpp \
    src/models/categoriesmodel.cpp \
    src/models/locationmodel.cpp \
    src/models/messagemodel.cpp \
    src/models/messagesortfiltermodel.cpp \
    src/models/servicemodel.cpp \
    src/service/serviceprovider.cpp \
    src/tools/messagehelper.cpp

DISTFILES += qml/harbour-apocalypse.qml \
    data/harbour.apocalypse.service \
    qml/cover/CoverPage.qml \
    qml/dialogs/LocationEditDialog.qml \
    qml/pages/AboutPage.qml \
    qml/pages/LocationListPage.qml \
    qml/pages/MessageListPage.qml \
    qml/pages/MessagePage.qml \
    qml/pages/OverviewPage.qml \
    qml/pages/ServiceListPage.qml \
    qml/pages/SettingsMapboxPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/SettingsSoundPage.qml \
    qml/pages/SettingsUpdatePage.qml \
    qml/tools/Helper.qml \
    rpm/harbour-apocalypse.changes \
    rpm/harbour-apocalypse.changes.run.in \
    rpm/harbour-apocalypse.spec \
    rpm/harbour-apocalypse.yaml \
    translations/*.ts \
    harbour-apocalypse.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 512x512

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-apocalypse-de.ts

RESOURCES += \
    ressources.qrc

HEADERS += \
    src/entities/location.h \
    src/entities/message.h \
    src/entities/service.h \
    src/models/categoriesmodel.h \
    src/models/locationmodel.h \
    src/models/messagemodel.h \
    src/models/messagesortfiltermodel.h \
    src/models/servicemodel.h \
    src/service/serviceprovider.h \
    src/tools/messagehelper.h

categoryIcons.files = icons/categories/*.svg
categoryIcons.path = $$INSTALL_ROOT/usr/share/harbour-apocalypse/icons/categories

coverBackground.files = icons/cover-background.svg
coverBackground.path = $$INSTALL_ROOT/usr/share/harbour-apocalypse/images

sound.files = data/siren.ogg
sound.path = $$INSTALL_ROOT/usr/share/harbour-apocalypse/sounds

INSTALLS += categoryIcons coverBackground sound
