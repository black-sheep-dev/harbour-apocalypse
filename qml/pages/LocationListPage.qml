import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.apocalypse 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Add Location")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/LocationEditDialog.qml"), {edit: false})

                    dialog.accepted.connect(function () {
                        ServiceProvider.locationModel().addLocation(dialog.name, dialog.latitude, dialog.longitude)
                        ServiceProvider.saveSettings()
                    })
                }
            }
        }

        id: listView
        model: ServiceProvider.locationModel()
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Locations")
        }

        delegate: ListItem {
            id: delegate
            width: parent.width
            contentHeight: Theme.itemSizeLarge

            menu: ContextMenu {

                MenuItem {
                    text: qsTr("Edit");
                    onClicked: {
                        var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/LocationEditDialog.qml"), {
                                                        edit: true,
                                                        name: name,
                                                        latitude: latitude,
                                                        longitude: longitude
                                                    })

                        dialog.accepted.connect(function () {
                            name = dialog.name
                            latitude = dialog.latitude
                            longitude = dialog.longitude
                            ServiceProvider.saveSettings()
                        })
                    }
                }
                MenuItem {
                    text: qsTr("Delete");
                    onClicked: remorse.execute(delegate, qsTr("Deleting location"), function() {
                        ServiceProvider.locationModel().removeLocation(index)
                        ServiceProvider.saveSettings();
                    })
                }
            }

            RemorseItem { id: remorse }

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
                        text: "lat: " + latitude + " / lon: " + longitude

                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }
            onClicked: console.log("Clicked " + index)
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No locations defined")
            hintText: qsTr("Pull down to add location")
        }

        VerticalScrollDecorator {}
    }
}
