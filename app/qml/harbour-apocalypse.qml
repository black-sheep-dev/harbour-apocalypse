import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import Nemo.DBus 2.0
import Nemo.Notifications 1.0

import "pages"

ApplicationWindow
{
    property bool loading: false
    property int localSeverity: 0
    property var localMessages: []
    property bool offline: true


    function updateLocalMessages() {
        localMessages = JSON.parse(dbusService.getProperty("localMessages"))
    }

    ConfigurationGroup {
        id: settings
        path: "/apps/harbour-apocalypse"
        synchronous: true

        property string mapboxApiKey
    }

    Notification {
        function show(message, icn) {
            replacesId = 0
            previewSummary = ""
            previewBody = message
            icon = icn || ""
            publish()
        }

        id: notification
        //% "Apocalypse"
        appName: qsTrId("id-app-name")
        expireTimeout: 3000
    }

    DBusInterface {
        id: dbusService
        service: 'org.nubecula.apocalypse'
        path:  '/'
        iface: 'org.nubecula.apocalypse.service'

        signalsEnabled: true
        propertiesEnabled: true

        function localMessagesChanged(msgs) {
            updateLocalMessages()
        }
        function loadingChanged() {
            loading = getProperty("loading")
        }
        function localSeverityChanged() {
            localSeverity = getProperty("localSeverity")
        }
        function notifyChanged(enabled) {
            console.log("Notify enabled: " + enabled)
        }
        function offlineChanged() {
            offline = getProperty("offline")
        }

        function playSoundChanged(enabled) {
            console.log("Play sound enabled: " + enabled)
        }

        Component.onCompleted: {
            localSeverity = getProperty("localSeverity")
            offline = getProperty("offline")
            updateLocalMessages()
        }
    }

//    DBusAdaptor {
//        id: dbusAdaptor
//        service: "org.nubecula.apocalypse.app"
//        iface: "org.nubecula.apocalypse.app"
//        path: "/"
//        xml: '\
//              <interface name="org.nubecula.apocalypse.app">
//                <method name="open">
//                    <arg name="msg" type="s" direction="in">
//                    </arg>
//                </method>
//              </interface>'

//        function open(msg) {
//            __silica_applicationwindow_instance.activate()
//            pageStack.push(Qt.resolvedUrl("pages/MessagePage.qml"), { msg: JSON.parse(msg) })
//        }
//    }

    initialPage: Component { OverviewPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: dbusService.call("refresh", undefined)
}
