import QtQuick 1.0

Rectangle {
    id: phaseView
    width: 400
    height: 32
    color: "#999999"
    radius: 0

    PhaseButton {
        id: untupButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        source: "TurnIcons/untap.png"
    }

    PhaseButton {
        id: upkeepButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: untupButton.right
        anchors.leftMargin: 0
        source: "TurnIcons/upkeep.png"
    }

    PhaseButton {
        id: drawButton
        x: 85
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: upkeepButton.right
        source: "TurnIcons/draw.png"
        anchors.leftMargin: 0
    }

    PhaseButton {
        id: beginningCombatButton
        width: 32
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: drawButton.right
        anchors.leftMargin: 16
        source: ""
    }

    PhaseButton {
        id: declareAtackersButton
        width: 32
        anchors.left: beginningCombatButton.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        source: ""
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
    }

    PhaseButton {
        id: declareBlockersButton
        width: 32
        anchors.left: declareAtackersButton.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        source: ""
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
    }

    PhaseButton {
        id: combatDamageButton
        width: 32
        anchors.left: declareBlockersButton.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        source: ""
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
    }

    PhaseButton {
        id: endcombatButton
        width: 32
        anchors.left: combatDamageButton.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        source: ""
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
    }

    PhaseButton {
        id: endTurnStepButton
        width: 32
        anchors.left: endcombatButton.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        source: ""
        anchors.bottomMargin: 0
        anchors.leftMargin: 16
    }

    PhaseButton {
        id: cleanupStepButton
        width: 32
        anchors.left: endTurnStepButton.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        source: ""
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
    }

    PhaseButton {
        id: doEndTurn
        width: 32
        anchors.left: cleanupStepButton.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        source: "TurnIcons/doendturn.png"
        anchors.bottomMargin: 0
        anchors.leftMargin: 16
    }
}
