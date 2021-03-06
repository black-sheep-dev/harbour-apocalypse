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
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem {
                text: qsTr("All Messages")
                onClicked: pageStack.push(Qt.resolvedUrl("MessageListPage.qml"))
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: ServiceProvider.refresh();
            }
        }

        id: listView

        opacity: ServiceProvider.loading ? 0.4 : 1.0
        Behavior on opacity { FadeAnimator {} }

        model: MessageSortFilterModel {
            id: sortModel
            sourceModel: ServiceProvider.messageModel()

            Component.onCompleted: showLocalOnly()
        }

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Local Messages")
        }
        delegate: ListItem {
            id: delegate

            width: parent.width
            contentHeight: contentColumn.height

            Column {
                id: contentColumn
                width: parent.width
                spacing: Theme.paddingSmall

                Row {
                    id: contentRow
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * x
                    spacing: Theme.paddingMedium

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

                    Column {
                        width: parent.width - itemIcon.width - Theme.paddingMedium
                        anchors.verticalCenter: itemIcon.verticalCenter

                        Label {
                            width: parent.width
                            text: model.locationName
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            width: parent.width
                            text: event_title.toUpperCase()
                            color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                            font.pixelSize: Theme.fontSizeMedium
                            wrapMode: Text.Wrap
                        }
                        Label {
                            text: sender_name

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeExtraSmall
                            wrapMode: Text.Wrap
                        }

                    }
                }
                Item {
                    visible: index < (listView.count - 1)
                    width: parent.width
                    height: 1
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl("MessagePage.qml"), { msg: ServiceProvider.messageModel().messageAt(idx) })
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No local messages")
            hintText: qsTr("There are no local messages available")
        }

        VerticalScrollDecorator {}
    }
}
