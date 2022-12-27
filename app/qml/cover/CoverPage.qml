import QtQuick 2.0
import Sailfish.Silica 1.0

import "../tools/"

CoverBackground {
    Helper { id: helper }

    Image {
        anchors {
            top: coverTitle.bottom
            topMargin: Theme.paddingSmall
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width
        height: sourceSize.height * width / sourceSize.width
        smooth: true
        source: "/usr/share/harbour-apocalypse/images/cover-background.svg"
        opacity: 0.1
    }

    BusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Medium
        running: loading
        anchors.centerIn: parent
    }

    Rectangle {
        id: background
        anchors.fill: parent;

        opacity: localSeverity === 0 ? 0.0 : 0.6

        color: helper.getSeverityColor(localSeverity)
    }


    Label {
        id: coverTitle

        anchors.horizontalCenter: parent.horizontalCenter
        y: Theme.paddingLarge

        //% "Apocalypse"
        text: qsTrId("id-app-name")
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
        anchors.bottom: parent.bottom
        width: parent.width
        clip: true

        model: localMessages

        delegate: ListItem {
            id: delegate

            width: parent.width
            contentHeight: Theme.itemSizeExtraSmall * 0.5

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                spacing: Theme.paddingMedium

                Image {
                    id: itemIcon

                    smooth: true
                    opacity: 0.4

                    width: Theme.itemSizeExtraSmall * 0.3
                    height: width

                    anchors.verticalCenter: parent.verticalCenter

                    sourceSize.width: 256
                    sourceSize.height: 256

                    source: helper.getCategoryIcon(modelData.category)
                }

                Label {
                    width: parent.width - itemIcon.width - Theme.paddingMedium
                    anchors.verticalCenter: itemIcon.verticalCenter

                    text: modelData.headline.toUpperCase()
                    font.pixelSize: Theme.fontSizeTiny
                    truncationMode: TruncationMode.Fade
                }
            }
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: dbusService.call("refresh", undefined)
        }
    }
}
