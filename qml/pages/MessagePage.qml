import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.apocalypse 1.0

Page {
    property Message msg

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width

            spacing: Theme.paddingSmall

            PageHeader {
                title: msg.eventTitle.toUpperCase()
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                font.pixelSize: Theme.fontSizeMedium
                font.bold: true
                color: Theme.highlightColor
                wrapMode: Text.WordWrap

                text: msg.headline
            }

            SectionHeader {
                text: qsTr("Details")

            }

            Row {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    width: parent.width * 0.3
                    text: qsTr("Sent:")
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    width: parent.width * 0.7
                    text: msg.sent.toLocaleString()
                    color: Theme.highlightColor
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }

            Row {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    width: parent.width * 0.3
                    text: qsTr("Type:")
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    width: parent.width * 0.7
                    text: {
                        switch (msg.type) {
                        case Message.Alert:
                            return qsTr("Alert")

                        case Message.Cancel:
                            return qsTr("Update")

                        case Message.Update:
                            return qsTr("Update")

                        default:
                            return qsTr("Unkown")
                        }
                    }
                    color: Theme.highlightColor
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }

            Row {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    width: parent.width * 0.3
                    text: qsTr("Severity:")
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    width: parent.width * 0.7
                    text: {
                        switch (msg.severity) {
                        case Message.SeverityMinor:
                            return qsTr("Minor")

                        case Message.SeverityModerate:
                            return qsTr("Moderate")

                        case Message.SeveritySevere:
                            return qsTr("Severe")

                        default:
                            return qsTr("Unkown")
                        }
                    }

                    color: Theme.highlightColor
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }

            SectionHeader {
                visible: msg.categories !== Message.CategoryNone
                text: qsTr("Categories")
            }

            Repeater {
                model: CategoriesModel {
                    categories: msg.categories
                }

                Label {
                    x : Theme.horizontalPageMargin
                    width: parent.width - 2*x

                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.highlightColor

                    text: title
                }
            }

            SectionHeader {
                text: qsTr("Description")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                linkColor: Theme.secondaryHighlightColor

                text: msg.description
            }

            SectionHeader {
                visible: msg.instruction.length > 0

                text: qsTr("Instruction")
            }

            Label {
                visible: msg.instruction.length > 0

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                text: msg.instruction
            }

            SectionHeader {
                visible: msg.contact.length > 0

                text: qsTr("Contact")
            }

            Label {
                visible: msg.contact.length > 0

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.WordWrap

                text: msg.contact
            }

            SectionHeader {
                visible: msg.web.length > 0

                text: qsTr("Web")
            }

            Label {
                visible: msg.web.length > 0

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.WordWrap

                text: msg.web

                onLinkActivated: Qt.openUrlExternally(msg.web)
            }

            Item {
                height: column.spacing
                width: 1
            }
        }

        VerticalScrollDecorator {}
    }
}

