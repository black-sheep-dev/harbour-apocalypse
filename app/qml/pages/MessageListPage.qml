import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0


import "../tools/"

Page {    
    id: page

    allowedOrientations: Orientation.All

    function refreshMessages() {
        listView.model = JSON.parse(dbusService.getProperty("messages"))
    }

    Helper { id: helper }

    BusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: loading
        anchors.centerIn: parent
    }

    SilicaListView {
        PullDownMenu {
            busy: loading
            MenuItem {
                //% "Refresh"
                text: qsTrId("id-refresh")
                onClicked: refreshMessages()
            }
        }

        id: listView

        opacity: loading ? 0.4 : 1.0
        Behavior on opacity { FadeAnimator {} }

        anchors.fill: parent
        header: PageHeader {
            //% "All messages"
            title: qsTrId("id-all-messages")
        }

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
            //% "No messages"
            text: qsTrId("id-no--msgs")
            //% "There are no messages available"
            hintText: qsTrId("id-no--msgs-hint")
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: refreshMessages()
}

