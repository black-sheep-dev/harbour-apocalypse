import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

import "../tools/"

Page {
    id: page

    allowedOrientations: Orientation.All

    Helper { id: helper }

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: loading
        anchors.centerIn: parent
    }

    SilicaListView {
        PullDownMenu {
            busy: loading
            MenuItem {
                //% "About"
                text: qsTrId("id-about")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                //% "Settings"
                text: qsTrId("id-settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem {
                //% "Show all"
                text: qsTrId("id-show-all")
                onClicked: pageStack.push(Qt.resolvedUrl("MessageListPage.qml"))
            }
            MenuItem {
                //% "Refresh"
                text: qsTrId("id-refresh")
                onClicked: dbusService.call("refresh", undefined)
            }
        }

        id: listView

        anchors.fill: parent
        header: PageHeader {
            //% "Local Messages"
            title: qsTrId("id-local-messages")
        }

        model: localMessages

        delegate: ListItem {
            id: delegate

            width: parent.width
            contentHeight: contentRow.height + 2*Theme.paddingSmall

            Row {
                id: contentRow
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                spacing: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIcon
                    smooth: true

                    opacity: 0.4

                    width: Theme.itemSizeSmall
                    height: width

                    sourceSize.width: 256
                    sourceSize.height: 256

                    source: helper.getCategoryIcon(modelData.category, modelData.severity)
                }

                Column {
                    width: parent.width - itemIcon.width - Theme.paddingMedium
                    spacing: Theme.paddingSmall

                    Label {
                        width: parent.width
                        text: modelData.locationName
                        font.pixelSize: Theme.fontSizeSmall
                        wrapMode: Text.Wrap
                    }
                    Label {
                        width: parent.width
                        text: modelData.headline.toUpperCase()
                        color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                        font.pixelSize: Theme.fontSizeSmall
                        font.bold: true
                        wrapMode: Text.Wrap
                    }
                    Label {
                        width: parent.width
                        text: modelData.sender
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: Text.Wrap
                    }
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl("MessagePage.qml"), { msg: modelData })
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            //% "No local messages"
            text: qsTrId("id-no-local-msgs")
            //% "There are no local messages available"
            hintText: qsTrId("id-no-local-msgs-hint")
        }

        VerticalScrollDecorator {}
    }
}
