import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    property bool edit: true
    property string name
    property double latitude: 0
    property double longitude: 0

    id: locationDialog

    canAccept: nameField.acceptableInput && latitudeField.acceptableInput && longitudeField.acceptableInput

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader {
            dialog: locationDialog
            title: edit ? qsTr("Edit location") : qsTr("Add location")
            acceptText: edit ? qsTr("Save") : qsTr("Add") 
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            wrapMode: Text.WordWrap

            color: Theme.highlightColor

            text: qsTr("Define a location by providing a coordinate with latitude and longitude.")
        }

        TextField {
            id: nameField
            width: parent.width
            placeholderText: qsTr("Enter location name")
            label: qsTr("Location name")

            validator: RegExpValidator {
                regExp: /^[a-zA-Z0-9ÄÖÜäöü_\- ]+$/gm
            }

            focus: !edit

            text: name

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: latitudeField.focus = true
        }

        TextField {
            id: latitudeField
            width: parent.width
            placeholderText: qsTr("Enter latitude (e.g. 52.518796)")
            label: qsTr("Latitude")

            inputMethodHints: Qt.ImhDigitsOnly
            validator: DoubleValidator {
                bottom: -90.0
                top: 90.0
                decimals: 16
                locale: Qt.locale("en_EN").name
            }

            text: edit ? latitude : ""

            onTextChanged: {
                text = text.replace(',', '.')
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: longitudeField.focus = true
        }

        TextField {
            id: longitudeField
            width: parent.width
            placeholderText: qsTr("Enter longitude (e.g. 13.376241)")
            label: qsTr("Longitude")

            inputMethodHints: Qt.ImhDigitsOnly
            validator: DoubleValidator {
                bottom: -180.0
                top: 180.0
                decimals: 16
                locale: Qt.locale("en_EN").name
            }

            text: edit ? longitude : ""

            onTextChanged: {
                text = text.replace(',', '.')
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: longitudeField.focus = false
        }
    }

    onDone: {
        name = nameField.text
        latitude = latitudeField.text
        longitude = longitudeField.text
    }
}
