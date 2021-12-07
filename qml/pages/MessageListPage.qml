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
            busy: ServiceProvider.loading
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

                    source: helper.getCategoryIcon(categories, severity)
                }

                Column {
                    width: parent.width - itemIcon.width - Theme.paddingMedium

                    Label {
                        width: parent.width
                        text: event_title.toUpperCase()
                        color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                        font.pixelSize: Theme.fontSizeSmall
                        font.bold: true
                        wrapMode: Text.Wrap
                    }
                    Label {
                        width: parent.width
                        text: sender_name
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: Text.Wrap
                    }
                }
            }
            onClicked: pageStack.animatorPush(Qt.resolvedUrl("MessagePage.qml"), { msg: ServiceProvider.messageModel().messageAt(idx) })
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No messages available")
            hintText: qsTr("Pull down to refresh messages")
        }

        VerticalScrollDecorator {}
    }
}

