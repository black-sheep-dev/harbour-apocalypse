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
                title: qsTr("Locations")
                description: qsTr("Manage locations info")
                icon: "image://theme/icon-m-location"
                page: "LocationListPage.qml"
            }
            ListElement {
                title: qsTr("Services")
                description: qsTr("Manage warning services")
                icon: "image://theme/icon-m-global-proxy"
                page: "ServiceListPage.qml"
            }
            ListElement {
                title: qsTr("Updates")
                description: qsTr("Manage automatic updates")
                icon: "image://theme/icon-m-sync"
                page: "SettingsUpdatePage.qml"
            }

//            ListElement {
//                title: qsTr("Developer Options")
//                description: qsTr("Manage developer options")
//                icon: "image://theme/icon-m-developer-mode"
//                page: ""
//            }
        }

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Settings")
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
