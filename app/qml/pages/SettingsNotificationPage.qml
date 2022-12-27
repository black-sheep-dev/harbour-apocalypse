import QtQuick 2.0
import Sailfish.Silica 1.0

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
                //% "Notification Settings"
                title: qsTrId("id-notification-settings")
            }

            TextSwitch {
                id: notifySwitch
                //% "Notify"
                text: qsTrId("id-notify")
                //% "Show a notification if there is a new warning in your area. You don't get warned if this is turned off!"
                description: qsTrId("id-notify-desc")

                onCheckedChanged: dbusService.setProperty('notify', checked)
                Component.onCompleted: checked = dbusService.getProperty('notify')
            }

            TextSwitch {
                id: playSoundSwitch
                enabled: notifySwitch.checked
                //% "Play notifaction sound"
                text: qsTrId("id-play-sound")
                //% "When active the application will play a sound when new notification is available."
                description: qsTrId("id-play-sound-desc")

                onCheckedChanged: dbusService.setProperty('playSound', checked)
                Component.onCompleted: checked = dbusService.getProperty('playSound')
            }
        }
    }
}
