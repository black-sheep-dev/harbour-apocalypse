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

# The name of your application
TARGET = harbour-apocalypse

QT += positioning

CONFIG += sailfishapp_qml

PKGCONFIG += nemonotifications-qt5 keepalive

DISTFILES += qml/harbour-apocalypse.qml \
    qml/cover/CoverPage.qml \
    qml/dialogs/LocationEditDialog.qml \
    qml/pages/AboutPage.qml \
    qml/pages/OverviewPage.qml \
    qml/pages/LocationListPage.qml \
    qml/pages/MessageListPage.qml \
    qml/pages/MessagePage.qml \
    qml/pages/ServiceListPage.qml \
    qml/pages/SettingsGpsPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/SettingsMapboxPage.qml \
    qml/pages/SettingsNotificationPage.qml \
    qml/pages/SettingsUpdatePage.qml \
    qml/tools/Helper.qml \
    qml/global.qml \
    qml/qmldir \
    rpm/harbour-apocalypse.changes.in \
    rpm/harbour-apocalypse.changes.run.in \
    rpm/harbour-apocalypse.spec \
    rpm/harbour-apocalypse.yaml \
    translations/*.ts \
    harbour-apocalypse.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

include(translations/translations.pri)

icons.files = icons/*.svg
icons.path = $$INSTALL_ROOT/usr/share/harbour-apocalypse/icons

categoryIcons.files = icons/categories/*.svg
categoryIcons.path = $$INSTALL_ROOT/usr/share/harbour-apocalypse/icons/categories

images.files = images/cover-background.svg
images.path = $$INSTALL_ROOT/usr/share/harbour-apocalypse/images

sounds.files = sounds/*
sounds.path = $$INSTALL_ROOT/usr/share/harbour-apocalypse/sounds

INSTALLS += icons categoryIcons images sounds
