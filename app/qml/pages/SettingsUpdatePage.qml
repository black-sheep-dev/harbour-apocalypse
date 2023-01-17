import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.KeepAlive 1.2

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
                //% "Automatic updates"
                title: qsTrId("id-automatic-updates")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                //% "Here you can activate / deactivate automatic updates of messages."
                text: qsTrId("id-update-desc-a")
                      + "\n"
                        //% "Activating these options leads to higher battery consumption and network traffic."
                      + qsTrId("id-update-desc-b");
            }


            TextSwitch {
                id: autoUpdateSwitch
                //% "Automatic updates"
                text: qsTrId("id-automatic-updates")
                //% "When active the application will update messages in a defined interval."
                description: qsTrId("id-automatic-updates-desc")

                onCheckedChanged: dbusService.setProperty("autoUpdate", checked)
                Component.onCompleted: checked = dbusService.getProperty("autoUpdate")
            }

            ComboBox {
                id: updateIntervalBox

                enabled: autoUpdateSwitch.checked

                width: parent.width
                //% "Update interval (minutes)"
                label: qsTrId("id-update-interval")

                menu: ContextMenu {
                    MenuItem {
                        text: "2.5"
                    }
                    MenuItem {
                        text: "5"
                    }
                    MenuItem {
                        text: "10"
                    }
                    MenuItem {
                        text: "15"
                    }
                    MenuItem {
                        text: "30"
                    }
                    MenuItem {
                        text: "60"
                    }
                }

                Component.onCompleted: {
                    const interval = dbusService.getProperty("updateInterval")

                    switch (interval) {
                    case BackgroundJob.TwoAndHalfMinutes:
                        currentIndex = 0
                        break

                    case BackgroundJob.FiveMinutes:
                        currentIndex = 1
                        break

                    case BackgroundJob.TenMinutes:
                        currentIndex = 2
                        break

                    case BackgroundJob.FifteenMinutes:
                        currentIndex = 3
                        break

                    case BackgroundJob.ThirtyMinutes:
                        currentIndex = 4
                        break

                    case BackgroundJob.OneHour:
                        currentIndex = 5
                        break

                    default:
                        currentIndex = 0
                        break
                    }
                }
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {

            const interval

            switch (updateIntervalBox.currentIndex) {
            case 0:
                interval = BackgroundJob.TwoAndHalfMinutes
                break

            case 1:
                interval = BackgroundJob.FiveMinutes
                break

            case 2:
                interval = BackgroundJob.TenMinutes
                break

            case 3:
                interval = BackgroundJob.FifteenMinutes
                break

            case 4:
                interval = BackgroundJob.ThirtyMinutes
                break

            case 5:
                interval = BackgroundJob.OneHour
                break

            default:
                interval = BackgroundJob.FiveMinutes
                break
            }

            dbusService.setProperty("updateInterval", interval)
        }
    }
}
