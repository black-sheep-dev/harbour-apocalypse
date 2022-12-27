import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: ListModel {
            id: listModel

            ListElement {
                //% "Locations"
                title: qsTrId("id-locations")
                //% "Manage locations info"
                description: qsTrId("id-locations-hint")
                icon: "image://theme/icon-m-location"
                page: "LocationListPage.qml"
            }
            ListElement {
                //% "Services"
                title: qsTrId("id-services")
                //% "Manage warning services"
                description: qsTrId("id-services-hint")
                icon: "image://theme/icon-m-global-proxy"
                page: "ServiceListPage.qml"
            }
            ListElement {
                //% "GPS"
                title: qsTrId("id-gps")
                //% "Manage gps settings"
                description: qsTrId("id-gps-hint")
                icon: "image://theme/icon-m-location"
                page: "SettingsGpsPage.qml"
            }
            ListElement {
                //% "Updates"
                title: qsTrId("id-updates")
                //% "Manage automatic updates"
                description: qsTrId("id-updates-hint")
                icon: "image://theme/icon-m-sync"
                page: "SettingsUpdatePage.qml"
            }
            ListElement {
                //% "Notification"
                title: qsTrId("id-notification")
                //% "Manage notifiction settings"
                description: qsTrId("id-notification-hint")
                icon: "image://theme/icon-m-speaker-on"
                page: "SettingsNotificationPage.qml"
            }
            ListElement {
                //% "Mapbox"
                title: qsTrId("id-mapbox")
                //% "Manage Mapbox API Key"
                description: qsTrId("id-mapbox-hint")
                icon: "image://theme/icon-m-levels"
                page: "SettingsMapboxPage.qml"
            }
        }

        anchors.fill: parent
        header: PageHeader {
            //% "Settings"
            title: qsTrId("id-settings")
        }
        delegate: ListItem {
            id: delegate
            width: parent.width
            contentHeight: Theme.itemSizeLarge

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIcon
                    source: icon
                    anchors.verticalCenter: parent.verticalCenter
                }

                Item {
                    width:Theme.paddingMedium
                    height:1
                }

                Column {
                    id: data
                    width: parent.width - itemIcon.width
                    anchors.verticalCenter: itemIcon.verticalCenter
                    Label {
                        id: text
                        width: parent.width
                        text: title
                        color: pressed?Theme.secondaryHighlightColor:Theme.highlightColor
                        font.pixelSize: Theme.fontSizeLarge
                    }
                    Label {
                        text: description
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl(page))
        }

        VerticalScrollDecorator {}
    }
}
