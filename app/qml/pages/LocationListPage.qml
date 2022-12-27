import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    function refreshModel() {
        var arr = JSON.parse(dbusService.getProperty("locations"))

        locationModel.clear()
        arr.forEach(function(item) {
            locationModel.append(item)
        })
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                //% "Add location"
                text: qsTrId("id-add-location")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/LocationEditDialog.qml"), {edit: false})

                    dialog.accepted.connect(function () {
                        dbusService.call("addLocation", [dialog.name, dialog.latitude, dialog.longitude], function (result) {
                            if (result) {
                                refreshModel()
                            } else {
                                //% "Failed to add location. Don't use name twice!"
                                notification.show(qsTrId("id-failed-to-add-location"))
                            }
                        },
                        function (error) {
                            //% "DBus error"
                            notification.show(qsTrId("id-dbus-error") + ": " + error)
                        })
                    })
                }
            }
        }

        id: listView
        model: ListModel { id: locationModel }
        anchors.fill: parent
        header: PageHeader {
            //% "Locations"
            title: qsTrId("id-locations")
        }

        delegate: ListItem {
            id: delegate
            width: parent.width
            contentHeight: Theme.itemSizeLarge

            menu: ContextMenu {

                MenuItem {
                    //% "Edit"
                    text: qsTrId("id-edit");
                    onClicked: {
                        var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/LocationEditDialog.qml"), {
                                                        edit: true,
                                                        name: name,
                                                        latitude: lat,
                                                        longitude: lon
                                                    })

                        dialog.accepted.connect(function () {
                            dbusService.call("updateLocation", [index, dialog.name, dialog.latitude, dialog.longitude], function (result) {
                                if (result) {
                                    refreshModel()
                                } else {
                                    //% "Failed to update location!"
                                    notification.show(qsTrId("id-failed-to-update-location"))
                                }
                            },
                            function (error) {
                                //% "DBus error"
                                notification.show(qsTrId("id-dbus-error") + ": " + error)
                            })
                        })
                    }
                }
                MenuItem {
                    //% "Delete"
                    text: qsTrId("id-delete");
                    //% "Deleting location"
                    onClicked: delegate.remorseAction(qsTrId("id-deleting-location"), function() {
                        dbusService.call("removeLocation", [index], function(result) {
                            if (result) {
                                refreshModel()
                            } else {
                                //% "Failed to remove location!"
                                notification.show(qsTrId("id-failed-to-remove-location"))
                            }
                        },
                        function (error) {
                            //% "DBus error"
                            notification.show(qsTrId("id-dbus-error") + ": " + error)
                        })
                    })
                }
            }

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIcon
                    source: "image://theme/icon-m-location"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Item {
                    width: Theme.paddingMedium
                    height: 1
                }

                Column {
                    width: parent.width - itemIcon.width - Theme.paddingMedium
                    anchors.verticalCenter: itemIcon.verticalCenter

                    Label {
                        width: parent.width
                        text: name
                        color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                        font.pixelSize: Theme.fontSizeLarge
                    }

                    Label {
                        text: "lat: " + lat.toFixed(6) + " / lon: " + lon.toFixed(6)

                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            //% "No locations defined"
            text: qsTrId("id-no-locations-defined")
            //% "Pull down to add location"
            hintText: qsTrId("id-no-locations-defined-hint")
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: refreshModel()
}
