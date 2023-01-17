import QtQuick 2.0
import Sailfish.Silica 1.0

import "../."

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                //% "About"
                title: qsTrId("id-about")
            }

            Image {
                id: logo
                source: "/usr/share/" + Global.appId + "/icons/" + Global.appId + ".svg"
                smooth: true
                sourceSize.width: parent.width / 2
                sourceSize.height: parent.width / 2
                height: parent.width / 2
                width: parent.width / 2
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                width: parent.width
                x : Theme.horizontalPageMargin
                font.pixelSize: Theme.fontSizeExtraLarge
                color: Theme.secondaryHighlightColor

                //% "Apocalypse"
                text: qsTrId("id-app-name")
            }

            Label {
                width: parent.width
                x : Theme.horizontalPageMargin
                text: Global.appVersion
            }

            Label {
                width: parent.width - 2 * x
                x : Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                //% "Apocalypse is an application for showing and notify of messages from German warning services like DWD, MOWAS, BIWAPP and KATWARN."
                text: qsTrId("id-about-text")
            }

            SectionHeader {
                //% "Social Media"
                text: qsTrId("id-social-media")
            }

            BackgroundItem {
                width: parent.width
                height: Theme.itemSizeMedium
                contentHeight: Theme.itemSizeMedium
                Row{
                    width:parent.width - 2 * x
                    height: parent.height
                    x:Theme.horizontalPageMargin
                    spacing:Theme.paddingLarge

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.height * 0.8
                        height: width
                        source: "/usr/share/" + Global.appId + "/icons/mastodon.svg"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "@" + Qt.application.name + "@social.nubecula.org"
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: {
                    //% "Copied to clipboard"
                    notification.show(qsTrId("id-copied-to-clipboard"))
                    Clipboard.text = "@" + Qt.application.name + "@social.nubecula.org"
                }
            }

            BackgroundItem {
                width: parent.width
                height: Theme.itemSizeMedium
                contentHeight: Theme.itemSizeMedium
                Row{
                    width:parent.width - 2 * x
                    height: parent.height
                    x:Theme.horizontalPageMargin
                    spacing:Theme.paddingLarge

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.height * 0.8
                        height: width
                        source: "/usr/share/" + Global.appId + "/icons/mastodon.svg"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "@blacksheep@social.nubecula.org"
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: {
                    //% "Copied to clipboard"
                    notification.show(qsTrId("id-copied-to-clipboard"))
                    Clipboard.text = "@blacksheep@social.nubecula.org"
                }
            }

            SectionHeader {
                //% "Sources"
                text: qsTrId("id-sources")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium
                Row{
                    width:parent.width - 2 * x
                    height: parent.height
                    x:Theme.horizontalPageMargin
                    spacing:Theme.paddingLarge

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.height * 0.8
                        height: width
                        source: "/usr/share/" + Global.appId + "/icons/github.svg"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "https://github.com/black-sheep-dev/" + Global.appId
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: Qt.openUrlExternally("https://github.com/black-sheep-dev/"  + Global.appId)
            }

            SectionHeader{
                //% "Donations"
                text: qsTrId("id-donations")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall

                //% "If you like my work why not buy me a beer?"
                text: qsTrId("id-buy-beer")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium

                Row{
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height
                    spacing:Theme.paddingLarge

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.height * 0.8
                        height: width
                        fillMode: Image.PreserveAspectFit
                        source: "/usr/share/" + Global.appId + "/icons/paypal.svg"
                    }
                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor
                        //% "Donate with PayPal"
                        text: qsTrId("id-donate-paypal")
                    }
                }
                onClicked: Qt.openUrlExternally("https://www.paypal.com/paypalme/nubecula/1")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium

                Row{
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height

                    spacing:Theme.paddingLarge

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.height * 0.8
                        height: width
                        fillMode: Image.PreserveAspectFit
                        source: "/usr/share/" + Global.appId + "/icons/liberpay.svg"
                    }
                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor
                        //% "Donate with Liberpay"
                        text: qsTrId("id-donate-liberpay")
                    }
                }
                onClicked: Qt.openUrlExternally("https://liberapay.com/black-sheep-dev/donate")
            }

            Item {
                width: 1
                height: Theme.paddingLarge
            }
        }
    }
}
