import QtQuick 2.0
import Sailfish.Silica 1.0

import QtPositioning 5.4
import MapboxMap 1.0

import "../tools/"

Page {
    property var msg

    id: page

    allowedOrientations: Orientation.All

    function fit() {
        map.fitView([
                        QtPositioning.coordinate(msg.boundingBox.x, msg.boundingBox.y),
                        QtPositioning.coordinate(msg.boundingBox.x + msg.boundingBox.width, msg.boundingBox.y + msg.boundingBox.height),
                    ])
    }

    Helper { id: helper }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width

            spacing: Theme.paddingMedium

            PageHeader {
                title: msg.headline.toUpperCase()
            }

            MapboxMap {
                id: map
                width: parent.width
                height: width

                minimumZoomLevel: 0
                maximumZoomLevel: 20
                pixelRatio: 3.0
                accessToken: settings.mapboxApiKey.length > 0 ? settings.mapboxApiKey : dbusService.getProperty("mapBoxApiKey")
                cacheDatabaseDefaultPath: true

                MapboxMapGestureArea {
                    id: mouseArea
                    map: map
                    activeClickedGeo: true
                    activeDoubleClickedGeo: true
                    activePressAndHoldGeo: false

                    onDoubleClicked: fit()
                }

                Component.onCompleted: {
                    setMargins(0.1, 0.1, 0.1, 0.1)

                    map.addSource("poly", {
                                    'type': 'geojson',
                                      'data': {
                                          'type': 'Feature',
                                          'geometry': {
                                              'type': 'Polygon',
                                              'coordinates': msg.polygons
                                          }
                                      }
                                  })

                    map.addLayer("overlay", {
                                     'id': 'poly',
                                     'type': 'fill',
                                     'source': 'poly',
                                     'layout': {},
                                     'paint': {
                                         'fill-color': helper.getSeverityColor(msg.severity),
                                         'fill-opacity': 0.5
                                     }
                                 })
                    map.addLayer("outline", {
                                     'id': 'outline',
                                     'type': 'line',
                                     'source': 'poly',
                                     'layout': {},
                                     'paint': {
                                         'line-color': helper.getSeverityColor(msg.severity),
                                         'line-width': 2
                                     }
                                 })


                    fit()
                }
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
                //% "Details"
                text: qsTrId("id-details")

            }

            DetailItem {
                //% "sent"
                label: qsTrId("id-sent")
                value: new Date(msg.sent).toLocaleString()
            }

            DetailItem {
                //% "Type"
                label: qsTrId("id-type")
                value: {
                    switch (msg.msgType) {
                    case "Alert":
                        //% "Alert"
                        return qsTrId("id-alert")

                    case "Cancel":
                        //% "Cancel"
                        return qsTrId("id-cancel")

                    case "Update":
                        //% "Update"
                        return qsTrId("id-update")

                    default:
                        //% "Unkown"
                        return qsTrId("id-unkown")
                    }
                }
            }

            DetailItem {
                //% "Severity"
                label: qsTrId("id-severity")
                value: {
                    switch (msg.severity) {
                    case 1:
                        //% "Minor"
                        return qsTrId("id-minor")

                    case 2:
                        //% "Moderate"
                        return qsTrId("id-moderate")

                    case 3:
                        //% "Severe"
                        return qsTrId("id-severe")

                    default:
                        //% "Unknown"
                        return qsTrId("id-unkown")
                    }
                }
            }

            SectionHeader {
                visible: msg.hasOwnProperty("category")
                //% "Categories"
                text: qsTrId("id-categories")
            }

            Repeater {
                model: msg.category

                Label {
                    x : Theme.horizontalPageMargin
                    width: parent.width - 2*x

                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.highlightColor

                    text: {
                        switch (modelData) {
                        case "Env":
                            //% "Enviornment"
                            return qsTrId("id-enviornment")

                        case "Fire":
                            //% "Fire"
                            return qsTrId("id-fire")

                        case "Geo":
                            //% "Geological"
                            return qsTrId("id-geological")

                        case "Health":
                            //% "Health"
                            return qsTrId("id-health")

                        case "Infra":
                            //% "Infrastructure"
                            return qsTrId("id-infrastructure")

                        case "Met":
                            //% "Meteorology"
                            return qsTrId("id-meteorology")

                        case "Other":
                            //% "Other"
                            return qsTrId("id-other")

                        case "Rescue":
                            //% "Rescue"
                            return qsTrId("id-rescue")

                        case "Safety":
                            //% "Safety"
                            return qsTrId("id-safety")

                        case "Security":
                            //% "Security"
                            return qsTrId("id-security")

                        default:
                            return modelData
                        }
                    }
                }
            }

            SectionHeader {
                //% "Description"
                text: qsTrId("id-description")
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
                visible: msg.hasOwnProperty("instruction")

                //% "Instruction"
                text: qsTrId("id-instruction")
            }

            Label {
                visible: msg.hasOwnProperty("instruction")

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                text: msg.instruction
            }

            SectionHeader {
                visible: msg.hasOwnProperty("contact")

                //% "Contact"
                text: qsTrId("id-contact")
            }

            Label {
                visible: msg.hasOwnProperty("contact")

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                text: msg.contact
            }

            SectionHeader {
                visible: msg.hasOwnProperty("web")

                //% "Web"
                text: qsTrId("id-web")
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                visible: msg.hasOwnProperty("web")
                //% "Browser"
                text: qsTrId("id-browser")
                onClicked: Qt.openUrlExternally(msg.web)
            }

            Label {
                visible: msg.hasOwnProperty("web")

                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

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

