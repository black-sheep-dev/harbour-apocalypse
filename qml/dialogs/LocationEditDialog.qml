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

            text: qsTr("Define a location by providing a coordiante with latitude and longitude.")
        }

        TextField {
            id: nameField
            width: parent.width
            placeholderText: qsTr("Enter location name")
            label: qsTr("Location name")

            validator: RegExpValidator {
                regExp: /^[a-zA-Z0-9_\- ]+$/gm
            }

            focus: !edit

            text: name

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: latitudeField.focus = true
        }

        TextField {
            id: latitudeField
            width: parent.width
            placeholderText: qsTr("Enter latitude")
            label: qsTr("Latitude")

            inputMethodHints: Qt.ImhDigitsOnly
//            validator: DoubleValidator {
//                bottom: -90.0
//                top: 90.0
//                decimals: 6
//                locale: Qt.locale().name
//            }

            text: latitude

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: longitudeField.focus = true
        }

        TextField {
            id: longitudeField
            width: parent.width
            placeholderText: qsTr("Enter longitude")
            label: qsTr("Longitude")

            inputMethodHints: Qt.ImhDigitsOnly
//            validator: DoubleValidator {
//                bottom: -180.0
//                top: 180.0
//                decimals: 6
//                locale: Qt.locale().name
//            }

            text: longitude

            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: longitudeField.focus = false
        }
    }

    Component.onCompleted: checkInput()

    onDone: {
        name = nameField.text
        latitude = latitudeField.text
        longitude = longitudeField.text
    }
}
