import QtQuick 1.1

Rectangle {
    id: cardView
    width: 300
    height: 433

    Image {
        id: backgroundImage
        anchors.fill: parent
        source: "qrc:/qtquickplugin/images/template_image.png"

        Item {
            id: headerRectangle
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: imageRectangle.top
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 22
            anchors.left: parent.left
            anchors.leftMargin: 22

            Text {
                id: nameText
                y: 6
                width: 80
                height: 20
                text: "Name"
                visible: true
                anchors.verticalCenterOffset: 3
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }
        }

        Item {
            id: imageRectangle
            height: 209
            anchors.top: parent.top
            anchors.topMargin: 27
            anchors.right: parent.right
            anchors.rightMargin: 22
            anchors.left: parent.left
            anchors.leftMargin: 22

            Image {
                id: cropImage
                anchors.fill: parent
                source: "qrc:/qtquickplugin/images/template_image.png"
            }
        }

        Item {
            id: middleRectangle
            height: 27
            anchors.top: imageRectangle.bottom
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 22
            anchors.left: parent.left
            anchors.leftMargin: 22

            Text {
                id: type
                y: 6
                width: 80
                height: 20
                text: "Type"
                anchors.verticalCenterOffset: 2
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            Text {
                id: edition
                x: 238
                y: 6
                width: 80
                height: 20
                text: "ED"
                anchors.right: parent.right
                anchors.rightMargin: -55
                font.pixelSize: 12
            }
        }

        Item {
            id: textRectangle
            anchors.top: middleRectangle.bottom
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 22
            anchors.left: parent.left
            anchors.leftMargin: 22
            anchors.bottom: bottomRectangle.top
            anchors.bottomMargin: 0

            Text {
                id: text
                y: 23
                height: 20
                text: "Text"
                wrapMode: Text.WordWrap
                anchors.right: parent.right
                anchors.rightMargin: 15
                anchors.left: parent.left
                anchors.leftMargin: 15
                font.pixelSize: 12
            }

            Text {
                id: flavor
//                y: 83
                y: text.y + text.height + 30
                height: 20
                text: "Flavor"
                smooth: false
                wrapMode: Text.WordWrap
                anchors.right: parent.right
                anchors.rightMargin: 15
                anchors.left: parent.left
                anchors.leftMargin: 15
                font.pixelSize: 12
            }
        }

        Item {
            id: bottomRectangle
            y: 397
            height: 36
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 22
            anchors.left: parent.left
            anchors.leftMargin: 22

            Text {
                id: artist
                y: 8
                width: 80
                height: 20
                text: "Artist"
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 0
                font.pixelSize: 12
            }

            Text {
                id: pt
                x: 232
                y: 8
                width: 80
                height: 20
                text: "P / T"
                anchors.right: parent.right
                anchors.rightMargin: -45
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }
        }
    }

    function setCard(card) {
        console.log("name: " + card.name);

        type.text = card.type;
        edition.text = card.edition;
        nameText.text = card.name;
        text.text = card.text;
        console.log(text.height);
        console.log(text.lineCount);
        flavor.text = card.flavor;
        artist.text = card.artist;

        if (card.power != ".")
            pt.text = card.power + " / " + card.toughness;
        else
            pt.text = "";


        backgroundImage.source = "/Users/arch/Programming/qt4/ccgo/src/Resources/backs/" + card.color +".jpg"

        cropImage.source = "/Users/arch/Programming/qt4/ccgo/src/" + card.name + ".png";
    }

}
