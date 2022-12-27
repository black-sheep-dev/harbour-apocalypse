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
                //% "GPS Settings"
                title: qsTrId("id-gps-settings")
            }

            TextSwitch {
                id: notifySwitch
                //% "GPS"
                text: qsTrId("id-gps")
                //% "When active the device will also warn you on actual position."
                description: qsTrId("id-gps-desc")

                onCheckedChanged: dbusService.setProperty('gps', checked)
                Component.onCompleted: checked = dbusService.getProperty('gps')
            }
        }
    }
}
