import QtQuick 1.0

Rectangle {
    id: container
    width: 32
    height: 32

    property variant source
    property variant text
    signal clicked


//    height: text.height + 10; width: text.width + 20
//    border.width: 1
//    radius: 4
//    smooth: true


//    SystemPalette { id: activePalette }

    Image {
        id: image
        source: parent.source
        anchors.fill: parent;
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: container.clicked()
    }

    Text {
        id: text
        anchors.centerIn:parent
        font.pointSize: 10
        text: parent.text
        color: activePalette.buttonText
    }
}
