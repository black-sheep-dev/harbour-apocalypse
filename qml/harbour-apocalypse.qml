import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.DBus 2.0
import Nemo.Configuration 1.0
import Nemo.Notifications 1.0
import Nemo.KeepAlive 1.2

import "pages"

import org.nubecula.harbour.apocalypse 1.0

ApplicationWindow
{
    DBusAdaptor {
        service: "harbour.apocalypse.service"
        iface: "harbour.apocalypse.service"
        path: "/harbour/apocalypse/service"
        xml: '\
  <interface name="harbour.apocalypse.service">
    <method name="open">
        <arg name="identifier" type="s" direction="in">
        </arg>
    </method>
  </interface>'

        function open(identifier) {
            __silica_applicationwindow_instance.activate()
            var msg = ServiceProvider.messageModel().messageByIdentifier(identifier)

            if (msg !== undefined)
                pageStack.push(Qt.resolvedUrl("pages/MessagePage.qml"), { msg: msg })
        }
    }

    ConfigurationGroup {
        id: settings
        path: "/apps/harbour-apocalypse"
        synchronous: true

        property bool autoUpdate: true
        property string mapboxApiKey
        property bool playSound: false
        property int updateInterval: BackgroundJob.FiveMinutes
    }

    BackgroundJob {
        enabled: settings.autoUpdate
        frequency: settings.updateInterval
        triggeredOnEnable: true

        onTriggered: {
            ServiceProvider.refresh()
            finished()
        }
    }

    initialPage: Component { OverviewPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: {
        ServiceProvider.playSound = settings.playSound
        ServiceProvider.initialize()
    }
}
