TARGET = harbour-apocalypse-daemon

VERSION = 0.4.2-1
DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += APP_NAME=\"\\\"$$TARGET\\\"\"

QT = core gui network dbus positioning

CONFIG += link_pkgconfig

PKGCONFIG += nemonotifications-qt5 keepalive

LIBS += -lz

SOURCES += src/harbour-apocalypse-daemon.cpp \
    src/apiinterface.cpp \
    src/serviceprovider.cpp

target.path = /usr/bin/

INSTALLS += target

HEADERS += \
    src/apiinterface.h \
    src/location.h \
    src/service.h \
    src/serviceprovider.h

RESOURCES += \
    ressources.qrc


