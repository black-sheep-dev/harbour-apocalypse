import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.apocalypse 1.0

import "../tools/"

CoverBackground {
    Helper { id: helper }

    Rectangle {
        id: background
        anchors.fill: parent;

        opacity: ServiceProvider.localSeverity === Message.SeverityUndefined ? 0.0 : 0.6

        color: helper.getSeverityColor(ServiceProvider.localSeverity)
    }

    Image {
        visible: ServiceProvider.localSeverity !== Message.SeverityUndefined

        id: icon
        anchors.centerIn: background

        opacity: 0.1

        smooth: true

        width: parent.width * 0.75
        height: width

        sourceSize.width: 256
        sourceSize.height: 256

        source: helper.getCategoryIcon(ServiceProvider.localMainCategories)
    }

    Label {
        id: coverTitle

        anchors.horizontalCenter: parent.horizontalCenter
        y: Theme.paddingLarge

        text: qsTr("Apocalypse")
        color: Theme.highlightColor
        font.pixelSize: Theme.fontSizeLarge
    }

    Item {
        id: spacer
        anchors.top: coverTitle.bottom
        width: 1
        height: Theme.paddingMedium
    }

    SilicaListView {
        id: listView

        anchors.top: spacer.bottom
        anchors.bottom: coverAction.top
        width: parent.width

        model: MessageSortFilterModel {
            id: filterModel
            sourceModel: ServiceProvider.messageModel()

            Component.onCompleted: showLocalOnly();
        }

        delegate: ListItem {
            id: delegate

            width: parent.width
            contentHeight: Theme.itemSizeExtraSmall

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIcon

                    smooth: true
                    opacity: 0.4

                    width: Theme.itemSizeExtraSmall * 0.5
                    height: width

                    anchors.verticalCenter: parent.verticalCenter

                    sourceSize.width: 256
                    sourceSize.height: 256

                    source: helper.getCategoryIcon(categories)
                }

                Item {
                    width: Theme.paddingMedium
                    height: 1
                }

                Label {
                    width: parent.width - itemIcon.width - Theme.paddingMedium
                    anchors.verticalCenter: itemIcon.verticalCenter

                    text: event_title.toUpperCase()
                    //color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No messages available")
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
        }
    }
}
