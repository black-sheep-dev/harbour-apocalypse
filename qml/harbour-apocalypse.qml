import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.DBus 2.0
import Nemo.Notifications 1.0

import "pages"

import org.nubecula.harbour.apocalypse 1.0

ApplicationWindow
{
    initialPage: Component { OverviewPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: ServiceProvider.initialize()

    DBusAdaptor {
        service: "harbour.apocalypse.service"
        iface: "harbour.apocalypse.service"
        path: "/harbour/apocalypse/service"
        xml: '\
  <interface name="harbour.apocalypse.service">
    <method name="open">
    </method>
  </interface>'

        function open() {
            __silica_applicationwindow_instance.activate()
        }
    }
}
