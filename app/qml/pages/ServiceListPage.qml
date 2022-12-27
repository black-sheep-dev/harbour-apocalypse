import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    id: page

    allowedOrientations: Orientation.All

    function refreshServices() {
        var arr = JSON.parse(dbusService.getProperty("services"))

        serviceModel.clear();
        arr.forEach(function(item) {
            serviceModel.append(item)
        })
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        header: PageHeader {
            //% "Services"
            title: qsTrId("id-services")
        }

        model: ListModel { id: serviceModel }

        delegate: BackgroundItem {
            id: delegate

            Label {
                id: nameLabel

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x - selectedIcon.width

                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            Image {
                id: selectedIcon
                anchors.left: nameLabel.right

                visible: active

                source: "image://theme/icon-m-acknowledge"
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: {
                dbusService.call("enableService", [id, !active], function (result) {
                    if (result) {
                        active = !active
                    } else {
                        //% "Failed to enable service"
                        notification.show(qsTrId("id-failed-enable-service"))
                    }
                },
                function (error) {
                    //% "DBus error"
                    notification.show(qsTrId("id-dbus-error") + ": " + error)
                })
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            //% "No contact to Apocalypse daemon"
            text: qsTrId("id-no-services-found")
            //% "Restart your device"
            hintText: qsTrId("id-no-services-defined-hint")
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: refreshServices()
}
