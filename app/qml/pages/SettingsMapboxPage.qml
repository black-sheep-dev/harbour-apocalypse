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

            width: parent.width

            spacing: Theme.paddingMedium

            PageHeader {
                //% "Mapbox"
                title: qsTrId("id-mapbox")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                wrapMode: Text.Wrap
                //% "You can provide your own Mapbox API key. This helps to reduce the number of API calls with the implementen key."
                text: qsTrId("id-provide-mapbox-key")
            }

            TextField {
                id: apiKeyField
                width: parent.width
                //% "API Key"
                label: qsTrId("id-api-key")
                color: Theme.highlightColor
                text: settings.mapboxApiKey
            }
        }
    }

    onStatusChanged: {
        if (status !== PageStatus.Deactivating) return

        if (apiKeyField.text.length === 0) return

        settings.mapboxApiKey = apiKeyField.text
    }
}
