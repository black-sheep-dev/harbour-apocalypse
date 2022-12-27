TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = app daemon

DISTFILES += \
    rpm/harbour-apocalypse.changes \
    rpm/harbour-apocalypse.spec

OTHER_FILES += \
    service/harbour-apocalypse.service

service.path = /usr/share/$${TARGET}
service.files = service

INSTALLS += dbus service
