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

                currentIndex: {
                    switch (dbusSerivce.getProperty("updateInterval")) {
                    case BackgroundJob.TwoAndHalfMinutes:
                        return 0

                    case BackgroundJob.FiveMinutes:
                        return 1

                    case BackgroundJob.TenMinutes:
                        return 2

                    case BackgroundJob.FifteenMinutes:
                        return 3

                    case BackgroundJob.ThirtyMinutes:
                        return 4

                    case BackgroundJob.OneHour:
                        return 5

                    default:
                        return 0
                    }
                }

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
