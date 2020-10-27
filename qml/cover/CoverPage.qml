import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.apocalypse 1.0

CoverBackground {
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
                    source: "image://theme/icon-m-media-radio"

                    anchors.verticalCenter: parent.verticalCenter
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
