import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.KeepAlive 1.2

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
                title: qsTr("Automatic Updates")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Here you can activate / deactivate automatic updates of messages.")
                      + "\n"
                      + qsTr("Activating these options leads to higher battery consumption and network traffic.");
            }


            TextSwitch {
                id: autoUpdateSwitch
                text: qsTr("Automatic Updates")
                description: qsTr("When active the application will update messages in a defined interval.")

                onCheckedChanged: settings.autoUpdate = checked
                Component.onCompleted: checked = settings.autoUpdate
            }

            ComboBox {
                id: updateIntervalBox

                enabled: autoUpdateSwitch.checked

                width: parent.width
                label: qsTr("Update interval (minutes)")

                currentIndex: {
                    switch (settings.updateInterval) {
                    case BackgroundJob.FiveMinutes:
                        return 0

                    case BackgroundJob.TenMinutes:
                        return 1

                    case BackgroundJob.FifteenMinutes:
                        return 2

                    case BackgroundJob.ThirtyMinutes:
                        return 3

                    case BackgroundJob.OneHour:
                        return 4

                    default:
                        return 0
                    }
                }

                menu: ContextMenu {
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
            switch (updateIntervalBox.currentIndex) {
            case 0:
                settings.updateInterval = BackgroundJob.FiveMinutes
                break

            case 1:
                settings.updateInterval = BackgroundJob.TenMinutes
                break

            case 2:
                settings.updateInterval = BackgroundJob.FifteenMinutes
                break

            case 3:
                settings.updateInterval = BackgroundJob.ThirtyMinutes
                break

            case 4:
                settings.updateInterval = BackgroundJob.OneHour
                break

            default:
                settings.updateInterval = BackgroundJob.FiveMinutes
                break
            }
        }
    }
}
