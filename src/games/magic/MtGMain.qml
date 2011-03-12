import QtQuick 1.0

Rectangle {
    id: rectangle2
    width: 800
    height: 600

    Rectangle {
        id: battlefieldOppRect
        y: 0
        height: 250
        color: "#ffffff"
        anchors.left: leftRect.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Image {
            id: image2
            fillMode: Image.Tile
            anchors.fill: parent
            source: "bkground.png"

            Image {
                id: image3
                x: 0
                y: 0
                width: 72
                height: 250
                source: "Resources/playerinfo.png"
            }
        }
    }

    Rectangle {
        id: handRect
        height: 100
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: leftRect.right
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    Rectangle {
        id: battlefieldRect
        color: "#ffffff"
        anchors.top: battlefieldOppRect.bottom
        anchors.topMargin: 0
        anchors.bottom: handRect.top
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: leftRect.right
        anchors.leftMargin: 0

        Image {
            id: image1
            fillMode: Image.Tile
            anchors.fill: parent
            source: "bkground.png"
        }
    }

    Rectangle {
        id: leftRect
        x: 0
        width: 150
        color: "#ffffff"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Rectangle {
            id: rectangle1
            x: 0
            y: 0
            width: 150
            height: 194
            color: "#ff0000"
        }

        Rectangle {
            id: rectangle3
            x: 0
            y: 390
            width: 150
            height: 210
            color: "#00ff00"
        }
    }

    PhaseView {
        id: phaseRect
        width: 400
        height: 32
        x: battlefieldRect.x + battlefieldRect.width/2 - width/2
        y: battlefieldRect.y - height/2
        color: "#ffffff"
    }
}
