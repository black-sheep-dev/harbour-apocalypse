import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.apocalypse 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x

            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Notification Sound")
            }

            TextSwitch {
                id: playSoundSwitch
                text: qsTr("Play notifaction sound")
                description: qsTr("When active the application will play a sound when new notification is available.")

                onCheckedChanged: settings.playSound = checked
                Component.onCompleted: checked = settings.playSound
            }
        }
    }
}
