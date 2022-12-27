import QtQuick 2.2
import Sailfish.Silica 1.0
import QtPositioning 5.0

Dialog {
    property bool edit: true
    property string name
    property double latitude: 0
    property double longitude: 0

    id: locationDialog

    canAccept: nameField.acceptableInput && latitudeField.acceptableInput && longitudeField.acceptableInput

    PositionSource {
        id: positionSource
        active: true
    }

    Column {
        width: parent.width
        spacing: Theme.paddingLarge

        DialogHeader {
            dialog: locationDialog
            title: edit ?
                       //% "Edit location"
                       qsTrId("id-edit-location") :
                       //% "Add location"
                       qsTrId("id-add-location")
            acceptText: edit ?
                            //% "Save"
                            qsTrId("id-save") :
                            //% "Add"
                            qsTrId("id-add")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            wrapMode: Text.WordWrap

            color: Theme.highlightColor

            //% "Define a location by providing a coordinate with latitude and longitude."
            text: qsTrId("id-add-location-desc")
        }

        TextField {
            id: nameField
            width: parent.width
            //% "Enter location name"
            placeholderText: qsTrId("id-enter-location-name")
            //% "Location name"
            label: qsTrId("id-location-name")

            validator: RegExpValidator {
                regExp: /^[a-zA-Z0-9ÄÖÜäöü_\- ]+$/gm
            }

            focus: !edit

            text: name

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: latitudeField.focus = true
        }

        Button {
            id: buttonCurrentPosition
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: positionSource.position.latitudeValid
            //% "Use current position"
            text: qsTrId("id-use-current-position")

            onClicked: {
                latitudeField.text = positionSource.position.coordinate.latitude
                longitudeField.text = positionSource.position.coordinate.longitude
            }
        }

        TextField {
            id: latitudeField
            width: parent.width
            //% "Enter latitude (e.g. 52.518796)"
            placeholderText: qsTrId("id-enter-latitude")
            //% "Latitude"
            label: qsTrId("id-latitude")

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
            //% "Enter longitude (e.g. 13.376241)"
            placeholderText: qsTrId("id-enter-longitude")
            //% "id-longitude"
            label: qsTrId("id-longitude")

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
