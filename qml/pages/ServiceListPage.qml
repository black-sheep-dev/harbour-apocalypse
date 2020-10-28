import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.apocalypse 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: ServiceProvider.serviceModel()
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Services")
        }
        delegate: BackgroundItem {
            id: delegate

            Label {
                id: nameLabel

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x - selectedIcon.width

                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            Image {
                id: selectedIcon
                anchors.left: nameLabel.right

                visible: active

                source: "image://theme/icon-m-acknowledge"
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: active = !active
        }
        VerticalScrollDecorator {}
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {
            ServiceProvider.saveSettings()
            ServiceProvider.refresh()
        }
    }
}
