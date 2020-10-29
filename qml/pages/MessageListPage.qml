import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

import org.nubecula.harbour.apocalypse 1.0

import "../tools/"

Page {
    id: page

    allowedOrientations: Orientation.All

    Helper { id: helper }

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: ServiceProvider.loading
        anchors.centerIn: parent
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: ServiceProvider.refresh();
            }
        }

        id: listView

        opacity: ServiceProvider.loading ? 0.4 : 1.0
        Behavior on opacity { FadeAnimator {} }

        model: ServiceProvider.messageModel()

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("All Messages")
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
                    smooth: true

                    opacity: 0.4

                    width: Theme.itemSizeSmall
                    height: width

                    anchors.verticalCenter: parent.verticalCenter

                    sourceSize.width: 256
                    sourceSize.height: 256

                    source: helper.getCategoryIcon(categories, severity)
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
                        text: event_title.toUpperCase()
                        color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                    Label {
                        text: sender_name

                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                }
            }
            onClicked: pageStack.push(Qt.resolvedUrl("MessagePage.qml"), { msg: ServiceProvider.messageModel().messageAt(idx) })
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No messages available")
            hintText: qsTr("Pull down to refresh messages")
        }

        VerticalScrollDecorator {}
    }
}

